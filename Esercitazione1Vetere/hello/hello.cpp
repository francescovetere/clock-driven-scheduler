#include <iostream>
#include <chrono>
#include <thread>

void print_hello()
{
	std::this_thread::sleep_for(std::chrono::seconds(1));
	std::cout << "Hello World!" << std::endl;
}

int main()
{
	std::thread my_thread(print_hello); // creo il nuovo thread, passandogli la funzione che deve eseguire
	my_thread.join(); // il thread main attende la conclusione dell'esecuzione del thread figlio my_thread
	//my_thread.detach();

	return 0;
}

