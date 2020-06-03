#include <cassert>
#include <iostream>
#include <chrono>

#include "rt/priority.h"
#include "rt/affinity.h"
#include "executive.h"

Executive::Executive(size_t num_tasks, unsigned int frame_length, unsigned int unit_duration)
	: p_tasks(num_tasks), frame_length(frame_length), unit_time(unit_duration), deadlines(p_tasks.size(), false), ap_request(false) {
}

void Executive::set_periodic_task(size_t task_id, std::function<void()> periodic_task, unsigned int wcet) {
	assert(task_id < p_tasks.size()); // Fallisce in caso di task_id non corretto (fuori range)
	
	p_tasks[task_id].function = periodic_task;
	p_tasks[task_id].wcet = wcet;
}

void Executive::set_aperiodic_task(std::function<void()> aperiodic_task, unsigned int wcet) {
 	ap_task.function = aperiodic_task;
 	ap_task.wcet = wcet;
}
		
void Executive::add_frame(std::vector<size_t> frame) {
	for (auto & id: frame)
		assert(id < p_tasks.size()); // Fallisce in caso di task_id non corretto (fuori range)
	
	frames.push_back(frame);

	/* ... */

	/* Calcolo lo slack time per questo frame, e lo inserisco nel vettore slack_times */
	unsigned int tot_wcet = 0; // wcet totale dei task periodici in questo frame

	for (unsigned int i = 0; i < frame.size(); ++i) {
		tot_wcet += p_tasks[frame[i]].wcet;
	}

	
	/* Lo slack time per questo frame sara' dato dalla differenza tra la lunghezza del frame e il wcet totale */
	unsigned int slack;
	if(frame_length > tot_wcet) slack = frame_length - tot_wcet;
	else slack = 0;

  	slack_times.push_back(slack);
}

void Executive::run() {

	// I thread dei vari task andranno mappati su un unico core
	rt::affinity affinity("1");

	// L'executive deve essere gestito da un thread a priorita' massima
	rt::priority prio_exec(rt::priority::rt_max);

	// Il task aperiodico deve essere gestito da un thread a priorita' minima
	rt::priority prio_aperiodic(rt::priority::rt_min);

	/* --- Creiamo i thread per i task periodici --- */
	for (size_t id = 0; id < p_tasks.size(); ++id)
	{
		assert(p_tasks[id].function); // Fallisce se set_periodic_task() non e' stato invocato per questo id
	
		p_tasks[id].thread = std::thread(&Executive::task_function, std::ref(p_tasks[id]));
		
		/* ... */
		
		// Assegniamo ad ogni thread periodico il valore di affinity precedentemente dichiarato
		rt::set_affinity(p_tasks[id].thread, affinity);
	}


	/* --- Creiamo il thread per il task aperiodico ---*/
	assert(ap_task.function); // Fallisce se set_aperiodic_task() non e' stato invocato
	
	ap_task.thread = std::thread(&Executive::task_function, std::ref(ap_task));
	
	rt::set_affinity(ap_task.thread, affinity);

	try {
		rt::set_priority(ap_task.thread, prio_aperiodic);
	}
	catch (rt::permission_error & e) {
		std::cerr << "Error setting RT priorities: " << e.what() << std::endl;
		ap_task.thread.detach();
	}

	
	/* --- Creiamo il thread per l'executive ---*/
	std::thread exec_thread(&Executive::exec_function, this);

	try {
		// Assegniamo all'executive la priorita' precedentemente dichiarata
		rt::set_priority(exec_thread, prio_exec);
	}
	catch (rt::permission_error & e) {
		std::cerr << "Error setting RT priorities: " << e.what() << std::endl;
		exec_thread.detach();
	}

	rt::set_affinity(exec_thread, affinity);
	

	/* ... */
	

	exec_thread.join();
	
	ap_task.thread.join();
	
	for (auto & pt: p_tasks)
		pt.thread.join();
}

void Executive::ap_task_request() {
	ap_request = true;
}	

void Executive::task_function(Executive::task_data & task) {
	while(true) {
		{	
			// Creiamo un blocco per sfruttare l'idioma RAII
			std::unique_lock<std::mutex> lock(task.mutex);

			if(task.state != task_state::PENDING)
				task.state = task_state::IDLE;

			// Serve solamente per il task aperiodico, in tutti gli altri casi non avra' effetto (overhead minimo)
			task.condition.notify_one();

			// Il task attende la ricezione di una notify_one per essere posto in esecuzione
			while(task.state != task_state::PENDING)
				task.condition.wait(lock);

			// Una volta giunto a questo punto, il task puo' essere posto in esecuzione
			task.state = task_state::RUNNING;

		}
		
		// Eseguiamo quindi la funzione associata al task
		task.function(); 
	}


}

void Executive::exec_function() {

	/* Frame corrente */
	unsigned int frame_id = 0;
	unsigned int hyperperiod_id = 0;

	// for(unsigned int i = 0; i < slack_times.size(); ++i) std::cout << "slack[" << i << "] = " << slack_times[i] << std::endl;

	/* ... */

	/* Istante assoluto che indica il prossimo risveglio dell'executive */
	auto wakeup = std::chrono::steady_clock::now(); 

	while (true) {
		std::cout << "\n === HYPERPERIOD: " << hyperperiod_id << " ===\n === FRAME: " << frame_id << " ===" << std::endl;

		auto start = std::chrono::high_resolution_clock::now();

		/* Controllo delle deadline... */

		// Evito il controllo sul primo frame del primo iperperiodo
		if(! (hyperperiod_id == 0 && frame_id == 0)) {

			unsigned int previous_frame;
			if(frame_id == 0) previous_frame = frames.size() - 1;
			else previous_frame = frame_id - 1; 

			for(unsigned int i = 0; i < (frames[previous_frame]).size(); ++i)
			{
				size_t task_id = (frames[previous_frame])[i];
				
				{
					std::unique_lock<std::mutex> lock(p_tasks[task_id].mutex);

					if(p_tasks[task_id].state != task_state::IDLE) {
						std::cerr << "*** Task " << task_id << ": deadline miss" << std::endl;
						deadlines[task_id] = true;
					}
				}
			}
		}

		/* Rilascio del task aperiodico (se necessario)... */
		if(ap_request) {
			ap_request = false;

			{
				std::unique_lock<std::mutex> lock(ap_task.mutex);
				if(ap_task.state != task_state::IDLE) {
						std::cerr << "*** Aperiodic task: deadline miss " << std::endl;
				}

				else {
					ap_task.state = task_state::PENDING;
					ap_task.condition.notify_one();
				}
			}
		}

		
		/* Rilascio dei task periodici del frame corrente */
		for(unsigned int i = 0; i < (frames[frame_id]).size(); ++i)
		{		
				size_t task_id = (frames[frame_id])[i];
				
				// Lascio uno slot di priorita' libero per l'eventuale aperiodico, di modo che possa assumere priorita'
				// superiore a tutti gli altri periodici, per poter fare slack stealing
				rt::priority prio_periodic(rt::priority::rt_max - i - 2);

				if(deadlines[task_id]) {
					deadlines[task_id] = false; // reset della deadline (ovvero permetto successive esecuzioni)
					std::cerr << "Task " << task_id <<": not executed because of a previous deadline miss" << std::endl;
				}

				else {

					{
						std::unique_lock<std::mutex> lock(p_tasks[task_id].mutex);
						p_tasks[task_id].state = task_state::PENDING;
					}

					try {
						rt::set_priority(p_tasks[task_id].thread, prio_periodic);
					}
				
					catch (rt::permission_error & e) {
						std::cerr << "Error setting RT priorities: " << e.what() << std::endl;
						p_tasks[task_id].thread.detach();
					}
					
					p_tasks[task_id].condition.notify_one();
			 	}
	  	}


		// if(slack_times[frame_id] > 0 && ap_task.state != task_state::IDLE) {
		if(slack_times[frame_id] > 0) {
			try {
				rt::set_priority(ap_task.thread, rt::priority::rt_max - 1);
			}
				
			catch (rt::permission_error & e) {
				std::cerr << "Error setting RT priorities: " << e.what() << std::endl;
				ap_task.thread.detach();
			}

			{
				std::unique_lock<std::mutex> lock(ap_task.mutex);
				// Si blocca al piu' fino a che dura lo slack time, ma se riceve una notify allora si sveglia prima!
				ap_task.condition.wait_until(lock, wakeup + (slack_times[frame_id])*(unit_time));
			}

			try {
				rt::set_priority(ap_task.thread, rt::priority::rt_min);
			}
				
			catch (rt::permission_error & e) {
				std::cerr << "Error setting RT priorities: " << e.what() << std::endl;
				ap_task.thread.detach();
			}
		}

		++frame_id;
		if (frame_id == frames.size()) {
			frame_id = 0;
			++hyperperiod_id;
		}

		/* Calcolo il nuovo istante assoluto di risveglio dell'executive */
		wakeup += std::chrono::milliseconds((unit_time) * (frame_length));

		/* Attesa assoluta, tale da non pregiudicare la precisione, fino al prossimo inizio frame */
		std::this_thread::sleep_until(wakeup);
		
		auto stop = std::chrono::high_resolution_clock::now();

		std::chrono::duration<double, std::milli> elapsed(stop - start);
		std::cout << "Elapsed [ms]: " << elapsed.count() << std::endl;
	}
	
}