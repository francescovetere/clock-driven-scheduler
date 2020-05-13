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
	/* TODO */
}

void PhiMonitor::putdown(size_t phi_id)
{
	/* TODO */
}


