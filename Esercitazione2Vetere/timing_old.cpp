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
		
		// genero la cadenza dei periodi
		auto point = std::chrono::steady_clock::now();
		
		for (unsigned int i = 0; i < 100; ++i)
		{
			do_some_stuff();
			
			point += std::chrono::seconds(1);
			std::this_thread::sleep_until(point);
			
			// uso questo clock per fare una misura il piu' precisa possibile sul particolare hw
			auto next = std::chrono::high_resolution_clock::now();
			
			std::chrono::duration<double, std::milli> elapsed(next - last);
			std::cout << "Time elapsed: " << elapsed.count() << "ms" << std::endl;
			
			/* if(elapsed > threshold) ... prendo qualche contromisura
				modo semplice, ma non bello! perchè mi sto accorgendo della deadline solamente a posteriori!
				serve un altro thread, che controlla in che stato si trova il mio thread che sto eseguendo per capire se c'è stata deadline
			*/
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

