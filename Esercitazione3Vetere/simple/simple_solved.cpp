#include <iostream>
#include <iomanip>
#include <list>
#include <thread>
#include <mutex>
#include <sstream>

const int N = 3;
const int M = 10;

int count = 0;
std::mutex mutex; 

void print(unsigned int id)
{
	for (int i = 0; i < N; ++i )
	{
		/******* Con lock ********/
		// mutex.lock();
		// int old_value = count;
		// int new_value = count + 1;
		// count = new_value;
		// mutex.unlock();

		// // cout può essere interrotta! meglio usare sstream
		// std::cout << "<child " << std::setw(2) << id << ">: value stored="
		// 	<< old_value << ", new value=" << new_value << std::endl;



		/******* Con lock e sstream ********/
		// mutex.lock();
		// int old_value = count;
		// int new_value = count + 1;
		// count = new_value;
		// mutex.unlock();

		// std::ostringstream ss;
		// ss << "<child " << std::setw(2) << id << ">: value stored="
		// 	<< old_value << ", new value=" << new_value << std::endl;

		// std::cout << ss.str();



		/******* Con unique_lock *******/
		// int old_value;
		// int new_value;
		// {
		// 	std::unique_lock<std::mutex> l(mutex);
		// 	old_value = count;
		// 	new_value = count + 1;
		// 	count = new_value;
		// }

		// std::cout << "<child " << std::setw(2) << id << ">: value stored="
		// 	<< old_value << ", new value=" << new_value << std::endl;




		/******* Con unique_lock e ostringstream *******/
		// std::ostringstream os;
		// int old_value;
		// int new_value;
		// {
		// 	std::unique_lock<std::mutex> l(mutex);
		// 	old_value = count;
		// 	new_value = count + 1;


		// 	os << "<child " << std::setw(2) << id << ">: value stored="
		// 		<< old_value << ", new value=" << new_value << std::endl;
		// 	count = new_value;
		// }

		// std::cout << os.str();
	}
}

int main()
{
	std::list<std::thread> childs;

	std::cout << "<main>: Starting child threads..." << std::endl;

	for (int id = 0; id < M; ++id)
		childs.emplace_back(print, id);

	for (auto it = childs.begin(); it != childs.end(); ++it)
		it -> join();

	std::cout << "<main>: value stored=" << count << "." << std::endl;

	if ( count == N * M )
		std::cout << "<main>:   Ok, good sons!!!" << std::endl;
	else
		std::cout << "<main>:   Uh, what's wrong?" << std::endl;

	return 0;
}

