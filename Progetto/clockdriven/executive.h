#ifndef EXECUTIVE_H
#define EXECUTIVE_H

#include <vector>
#include <functional>
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>

class Executive
{
	public:
		/* Inizializza l'executive, impostando i parametri di scheduling:
			num_tasks: numero totale di task presenti nello schedule;
			frame_length: lunghezza del frame (in quanti temporali);
			unit_duration: durata dell'unita di tempo, in millisecondi (default 10ms).
		*/
		Executive(size_t num_tasks, unsigned int frame_length, unsigned int unit_duration = 10);

		/* Imposta il task periodico di indice "task_id" (da invocare durante la creazione dello schedule):
			task_id: indice progressivo del task, nel range [0, num_tasks);
			periodic_task: funzione da eseguire al rilascio del task;
			wcet: tempo di esecuzione di caso peggiore (in quanti temporali).
		*/
		void set_periodic_task(size_t task_id, std::function<void()> periodic_task, unsigned int wcet);
		
		/* Imposta il task aperiodico (da invocare durante la creazione dello schedule):
			aperiodic_task: funzione da eseguire al rilascio del task;
			wcet: tempo di esecuzione di caso peggiore (in quanti temporali).
		*/
		void set_aperiodic_task(std::function<void()> aperiodic_task, unsigned int wcet);
		
		/* Lista di task da eseguire in un dato frame (da invocare durante la creazione dello schedule):
			frame: lista degli id corrispondenti ai task da eseguire nel frame, in sequenza
		*/
		void add_frame(std::vector<size_t> frame);

		/* Esegue l'applicazione */
		void run();
		
		/* Richiede il rilascio del task aperiodico (da invocare durante l'esecuzione).
		*/
		void ap_task_request();

	private:

		/* Usiamo una enum class piuttosto che una enum semplice */
		enum class task_state {PENDING, IDLE, RUNNING};
		
		struct task_data
		{
			std::function<void()> function;
			unsigned int wcet;

			std::thread thread;

			/* ... */

			/* Per modellare i rilasci in modo efficiente,
			   ogni task ha bisogno di un mutex e una condition variable */
			std::mutex mutex;
			std::condition_variable condition;

			/* Ogni task ha uno stato associato */
			task_state state;
		};
		
		std::vector<task_data> p_tasks;
		task_data ap_task;
		
		std::vector< std::vector<size_t> > frames;
		
		
		const unsigned int frame_length; // lunghezza del frame (in quanti temporali)
		const std::chrono::milliseconds unit_time; // durata dell'unita di tempo (quanto temporale)
		
		/* ... */

		/* Mantengo un vettore di booleani, in cui deadlines[i] = true indica che il task i-esimo ha subìto una deadline miss 
		   Inizializzo il vettore con n valori false, dove n = p_tasks.size() */
		std::vector<bool> deadlines;

		static void task_function(task_data & task);
		
		void exec_function();
};

#endif
