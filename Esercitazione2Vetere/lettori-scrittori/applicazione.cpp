#include <iostream>
#include <iomanip>
#include <thread>
#include <list>
#include <chrono>
#include <sstream>
#include <random>
#include <atomic>

#include "monitor.h"

const unsigned int NUM_LETTORI = 10;
const unsigned int NUM_SCRITTORI = 5;

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> dis(1, 42);
std::atomic_uint last_id(dis(gen));

unsigned int get_uid()
{
	return last_id += dis(gen);
}

void lettore(RWMonitor & monitor)
{
	unsigned int uid = get_uid();

	std::ostringstream name;
	std::ostringstream would;
	std::ostringstream start;
	std::ostringstream stop;

	name << "new reader uid=" << std::setw(4) << std::setfill('0') << uid << std::endl;
	would << std::setw(4) << std::setfill('0') << uid << " would read" << std::endl;
	start << std::setw(4) << std::setfill('0') << uid << " start reading" << std::endl;
	stop  << std::setw(4) << std::setfill('0') << uid << " stop reading" << std::endl;

	std::cout << name.str();

	while(true)
	{
		std::cout << would.str();

		monitor.rlock();

		std::cout << start.str();

		// Placeholder for "real world" code
		std::this_thread::sleep_for(std::chrono::seconds(2 + uid % NUM_LETTORI));

		std::cout << stop.str();

		monitor.runlock();

		// Placeholder for "real world" code
		std::this_thread::sleep_for(std::chrono::seconds(1 + uid % NUM_LETTORI));
	}
}

void scrittore(RWMonitor & monitor)
{
	unsigned int uid = get_uid();

	std::ostringstream name;
	std::ostringstream would;
	std::ostringstream start;
	std::ostringstream stop;

	name << "new writer uid=" << std::setw(4) << std::setfill('0') << uid << std::endl;
	would << std::setw(4) << std::setfill('0') << uid << " would write" << std::endl;
	start << std::setw(4) << std::setfill('0') << uid << " start writing" << std::endl;
	stop  << std::setw(4) << std::setfill('0') << uid << " stop writing" << std::endl;

	std::cout << name.str();

	while(true)
	{
		std::cout << would.str();

		monitor.wlock();

		std::cout << start.str();

		// Placeholder for "real world" code
		std::this_thread::sleep_for(std::chrono::seconds(1 + uid % NUM_SCRITTORI));

		std::cout << stop.str();

		monitor.wunlock();

		// Placeholder for "real world" code
		std::this_thread::sleep_for(std::chrono::seconds(10 + uid % NUM_SCRITTORI));
	}
}


int main()
{
	RWMonitor monitor;
	std::list<std::thread> rwthreads;

	for(unsigned int i = 0; i < NUM_LETTORI; ++i)
		rwthreads.emplace_back(lettore, std::ref(monitor));

	for(unsigned int i = 0; i < NUM_SCRITTORI; ++i)
		rwthreads.emplace_back(scrittore, std::ref(monitor));

	// wait for threads (forever...)
	for (auto & th : rwthreads)
		th.join();

	return 0;
}

