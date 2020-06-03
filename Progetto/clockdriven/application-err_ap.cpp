#include <iostream>

#include "executive.h"
#include "busy_wait.h"

/* Riporta la sitazione nel pdf di esempio, in cui pero' il task aperiodico e' richiesto dal task 1 (qui avente indice 0) 
   Questo porta l'aperiodico ad essere eseguito troppe volte, in particolare capitano dei suoi rilasci nonostante precedenti sue esecuzioni
   non siano ancora terminate (deadline miss) */

Executive exec(5, 4);

int wcet[] = {1, 2, 1, 3, 1};
int ap_task_wcet = 2;

/* Teniamo un fattore moltiplicativo minore di 10, per evitare deadline miss non volute, dovute ad overhead */
int fact = 8;

unsigned int count = 0;

void task0() 
{
	std::cout << "Task 0: executing" << std::endl;
	busy_wait((wcet[0]*fact) / 2);

	/* Richiede all'executive di eseguire un aperiodico, che devo avere settato nel main
	   verrà eseguito a partire dal frame successivo 
	*/
	if(++count % 2 == 0)
		exec.ap_task_request(); 

	busy_wait((wcet[0]*fact) / 2);
	std::cout << "Task 0: completed\n" << std::endl;
}

void task1()
{	
	std::cout << "Task 1: executing" << std::endl;
	busy_wait(wcet[1]*fact);
	std::cout << "Task 1: completed\n" << std::endl;
}

void task2()
{
	std::cout << "Task 2: executing" << std::endl;
	busy_wait(wcet[2]*fact);
	std::cout << "Task 2: completed\n" << std::endl;
}

void task3()
{
	std::cout << "Task 3: executing" << std::endl;
	busy_wait(wcet[3]*fact);
	std::cout << "Task 3: completed\n" << std::endl;
}

void task4()
{
	std::cout << "Task 4: executing" << std::endl;
	busy_wait(wcet[4]*fact);
	std::cout << "Task 4: completed\n" << std::endl;
}

/* Nota: nel codice di uno o piu' task periodici e' lecito chiamare Executive::ap_task_request() */

void ap_task()
{
	std::cout << "Aperiodic task: executing" << std::endl;
	busy_wait(ap_task_wcet*fact);
	std::cout << "Aperiodic task: completed" << std::endl;
}

int main()
{
	busy_wait_init();

	// Associo ad ogni id dei task, una rispettiva funzione
	exec.set_periodic_task(0, task0, wcet[0]); // tau_1
	exec.set_periodic_task(1, task1, wcet[1]); // tau_2
	exec.set_periodic_task(2, task2, wcet[2]); // tau_3,1
	exec.set_periodic_task(3, task3, wcet[3]); // tau_3,2
	exec.set_periodic_task(4, task4, wcet[4]); // tau_3,3
	/* ... */
	
	exec.set_aperiodic_task(ap_task, ap_task_wcet);
	
	exec.add_frame({0,1,2});
	exec.add_frame({0,3});
	exec.add_frame({0,1});
	exec.add_frame({0,1});
	exec.add_frame({0,1,4});
	/* ... */
	
	exec.run();
	
	return 0;
}
