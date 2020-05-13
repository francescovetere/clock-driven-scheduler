#include <iostream>
#include <iomanip>
#include <list>
#include <thread>

const int N = 3;
const int M = 10;

int count = 0;

void print(unsigned int id)
{
	for (int i = 0; i < N; ++i )
	{
		int old_value = count;
		int new_value = count + 1;

		std::cout << "<child " << std::setw(2) << id << ">: value stored="
			<< old_value << ", new value=" << new_value << std::endl;

		count = new_value;
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

