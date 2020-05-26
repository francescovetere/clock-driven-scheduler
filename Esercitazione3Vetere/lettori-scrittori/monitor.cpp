#include "monitor.h"

RWMonitor::RWMonitor() : num_readers(0), num_writers(0), readers_in_queue(0), writers_in_queue(0)
{
}

void RWMonitor::rlock()
{
	/* TODO */
	std::unique_lock<std::mutex> lock(mutex);

	++readers_in_queue;

	// strong writer preference
	while(num_writers > 0 || writers_in_queue > 0)
		read_queue.wait(lock);

	--readers_in_queue;

	++num_readers;

	// wake up chain
	read_queue.notify_one();
}

void RWMonitor::runlock()
{
	/* TODO */
	std::unique_lock<std::mutex> lock(mutex);

	--num_readers;

	if(num_readers == 0)
		write_queue.notify_one();
}


void RWMonitor::wlock()
{
	/* TODO */
	std::unique_lock<std::mutex> lock(mutex);

	++writers_in_queue;

	while(num_readers > 0 || num_writers > 0)
		write_queue.wait(lock);

	--writers_in_queue;

	num_writers = 1;
}

void RWMonitor::wunlock()
{
	/* TODO */
	std::unique_lock<std::mutex> lock(mutex);

	num_writers = 0;

	// strong writer preference
	if( writers_in_queue > 0 )
		write_queue.notify_one();
	else
		read_queue.notify_one();
}

