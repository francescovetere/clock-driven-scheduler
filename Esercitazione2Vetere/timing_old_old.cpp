#include <iostream>
#include <chrono>
#include <thread>
#include <list>
#include <random>

#include "rt/priority.h"

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> dis(10, 50);

void do_some_stuff()
{
	auto stop = std::chrono::high_resolution_clock::now();
	stop += std::chrono::milliseconds(dis(gen));
	
	while (std::chrono::high_resolution_clock::now() < stop)
	{
	// busy wait
	}
}

int main()
{
	try
	{
		rt::this_thread::set_priority(rt::priority::rt_max);
		
		auto last = std::chrono::high_resolution_clock::now();
		
		for (unsigned int i = 0; i < 100; ++i)
		{
			do_some_stuff();
			
			std::this_thread::sleep_for(std::chrono::seconds(1));
			
			auto next = std::chrono::high_resolution_clock::now();
			
			std::chrono::duration<double, std::milli> elapsed(next - last);
			std::cout << "Time elapsed: " << elapsed.count() << "ms" << std::endl;
			
			last = next;
		}
		
		return 0;
	}
	catch (rt::permission_error & e)
	{
		std::cerr << "Error setting RT priorities: " << e.what() << std::endl;
		return -1;
	}
}

