#include <iostream>
#include <thread>
#include <deque>
#include <list>
#include <chrono>
#include <sstream>
#include <string>
#include <mutex>
#include <condition_variable>

// Uncomment to enable RT priorities for threads A & B
//#define USE_RT_PRIO // usare sudo!!!

#ifdef USE_RT_PRIO
#include "rt/priority.h"
#include "rt/affinity.h"
#endif

void do_some_stuff(const std::string & name, char data, unsigned millisec)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(millisec));

	std::ostringstream str;
	str << name << ": '" << data << '\'' << std::endl;
	std::cout << str.str();
}

// Shared data ..............
std::deque<char> buffer;
std::mutex mutex;
std::condition_variable cond;
// ..........................

void thread_a()
{
	char val = 0;
	
	do
	{
		{
			std::unique_lock<std::mutex> lock(mutex);

			while( buffer.empty() ) // if sarebbe sbagliato
				cond.wait(lock);

			val = buffer.front();
			buffer.pop_front();
		}
		do_some_stuff("Thread A", val, 500);
	}
	while (val != '\n');
}

void thread_b()
{
	const std::string hello = "Hello World!\n";
	
	for (auto & v : hello)
	{
		do_some_stuff("Thread B", v, 200);

		{
			std::unique_lock<std::mutex> lock(mutex);
			buffer.push_back(v);
		
		

			if (!buffer.empty()) //posso ometterla, sicuramente se ho inserito, buffer non è empty
				cond.notify_all(); // non è detto che risvegli immediatamente il thread addormentato!!!!! 
		}
	}
}
		
int main()
{

#ifdef USE_RT_PRIO
	rt::priority prio(rt::priority::rt_max);
	rt::affinity aff("1");

	rt::this_thread::set_affinity(aff);

	try
	{
		rt::this_thread::set_priority(prio--);
	}
	catch (rt::permission_error &)
	{
		std::cerr << "Warning: RT priorities are not available"<< std::endl;
	}
#endif

	std::thread th_a(thread_a);
	std::thread th_b(thread_b);	
	
#ifdef USE_RT_PRIO
	rt::set_affinity(th_a, aff);
	rt::set_affinity(th_b, aff);
		
	try
	{
		rt::set_priority(th_a, prio--);
		rt::set_priority(th_b, prio);
	}
	catch (rt::permission_error &)
	{
	}
#endif

	th_a.join();
	th_b.join();
	
	return 0;
}

