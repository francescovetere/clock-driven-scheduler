#include "monitor.h"

PhiMonitor::PhiMonitor(size_t num) : cond(num), state(num, THINKING)
{
}

inline size_t PhiMonitor::left(size_t id) const
{
	return (id + size() - 1) % size();
}

inline size_t PhiMonitor::right(size_t id) const
{
	return (id + 1) % size();
}

void PhiMonitor::pickup(size_t phi_id)
{
	std::unique_lock<std::mutex> lock(mutex);

	// Mi dichiaro hungry
	state[phi_id] = HUNGRY;

	// Attendo sulla MIA variabile di condizione finchè i filosofi a sx e dx stanno mangiando
	while((state[left(phi_id)] == EATING ) || (state[right(phi_id)] == EATING ))
		cond[phi_id].wait(lock); 

	// Una volta giunto a questo punto, posso dichiararmi eating
	state[phi_id] = EATING;
}

void PhiMonitor::putdown(size_t phi_id)
{
	std::unique_lock<std::mutex> lock(mutex);

	// Mi dichiaro subito disinteressato a mangiare, ovvero thinking
	state[phi_id] = THINKING;

	// Risveglio il filosofo a sx (sulla SUA variabile di condizione) se lui è affamato,
	// e inoltre il filosofo alla sua sinistra non sta già mangiando
	if(state[left(phi_id)] == HUNGRY && state[left(left(phi_id))] != EATING)
		cond[left(phi_id)].notify_one();

	// Risveglio il filosofo a dx con un ragionamento analogo
	if(state[right(phi_id)] == HUNGRY && state[right(right(phi_id))] != EATING)
		cond[right(phi_id)].notify_one();
}


