#include "executive.h"
#include "busy_wait.h"

Executive exec(5, 4);

void task0()
{
	/* Custom Code */
}

void task1()
{
	/* Custom Code */
}

void task2()
{
	/* Custom Code */
}

void task3()
{
	/* Custom Code */
}

void task4()
{
	/* Custom Code */
}

/* Nota: nel codice di uno o piu' task periodici e' lecito chiamare Executive::ap_task_request() */

void ap_task()
{
	/* Custom Code */
}

int main()
{
	busy_wait_init();

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
