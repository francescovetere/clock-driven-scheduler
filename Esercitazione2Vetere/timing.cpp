#include <iostream>
#include <chrono>
#include <thread>
#include <list>
#include <random>

#include "rt/priority.h"

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> dis(10, 50);

int fibonacci (int n) {
    if (n == 0 || n == 1) return 1;
        
    int a = 0;
    int b = 1;
    int res = 0;
        
    for (int i = 2; i <= n; ++i) {
        res = a + b;
        a = b;
        b = res;
    }
	
	return res;   
}

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
		
		auto point = std::chrono::steady_clock::now();
		
		unsigned int cycles = 8;

		for (unsigned int i = 1; i <= cycles; ++i) {
			do_some_stuff();
			
			point += std::chrono::seconds(fibonacci(i));
			std::this_thread::sleep_until(point);
			
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

