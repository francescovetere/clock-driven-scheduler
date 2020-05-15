#include <iostream>
#include <chrono>
#include <thread>
#include <list>
#include <random>

#include "barrier.h"

const std::string hello("Hello World!");

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> dis(10, 50);

void print_char(size_t i, barrier & b)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(dis(gen)));
	
	b.wait();

	std::cout << hello[i];
	std::cout.flush();
}

int main()
{
	barrier b(hello.size());
	
	std::list<std::thread> threads;

	for(size_t id = 0; id < hello.size(); ++id)
	{
		std::thread th(print_char, id, std::ref(b));
		threads.push_back(std::move(th));
	}

	for (auto & th : threads)
		th.join();

	std::cout << std::endl;

	return 0;
}

