#include <iostream>
#include <chrono>
#include <thread>
#include <list>
#include <random>

#include "rt/priority.h"

#define TIMES (8)

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> dis(10, 50);

//Get n# number of fibonacci serie
int get_n_fibonacci(int n);

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
		
		for (unsigned int i = 1; i <= TIMES; ++i)
		{
			do_some_stuff();
			
			point += std::chrono::seconds(get_n_fibonacci(i));
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

int get_n_fibonacci(int n)
{
	int cont,i=0,j=1,k=0;

	if (n==1)
		return 1;

	for(cont = 0; cont<(n-1); ++cont)
	{
		k = i+j;
		i=j;
		j=k;
	}
	return k;
}
