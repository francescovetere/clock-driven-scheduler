#include <iostream>

#include "executive.h"
#include "busy_wait.h"

// 5 frame, ognuno con ampiezza 4 unita' di tempo
Executive exec(5, 4);

void task0() 
{
	std::cout << "Task 0: executing" << std::endl;
	busy_wait(1);
	std::cout << "Task 0: completed\n" << std::endl;
}

void task1()
{	
	std::cout << "Task 1: executing" << std::endl;
	busy_wait(2);
	std::cout << "Task 1: completed\n" << std::endl;
}

void task2()
{
	std::cout << "Task 2: executing" << std::endl;
	busy_wait(1);
	std::cout << "Task 2: completed\n" << std::endl;
}

void task3()
{
	std::cout << "Task 3: executing" << std::endl;
	busy_wait(3);
	std::cout << "Task 3: completed\n" << std::endl;
}

void task4()
{
	std::cout << "Task 4: executing" << std::endl;
	busy_wait(1);
	std::cout << "Task 4: completed\n" << std::endl;
}

/* Nota: nel codice di uno o piu' task periodici e' lecito chiamare Executive::ap_task_request() */

void ap_task()
{
	/* Custom Code */
}

int main()
{
	busy_wait_init();

	// Associo ad ogni id dei task, una rispettiva funzione
	exec.set_periodic_task(0, task0, 1); // tau_1
	exec.set_periodic_task(1, task1, 2); // tau_2
	exec.set_periodic_task(2, task2, 1); // tau_3,1
	exec.set_periodic_task(3, task3, 3); // tau_3,2
	exec.set_periodic_task(4, task4, 1); // tau_3,3
	/* ... */
	
	exec.set_aperiodic_task(ap_task, 2);
	
	exec.add_frame({0,1,2});
	exec.add_frame({0,3});
	exec.add_frame({0,1});
	exec.add_frame({0,1});
	exec.add_frame({0,1,4});
	/* ... */
	
	exec.run();
	
	return 0;
}
