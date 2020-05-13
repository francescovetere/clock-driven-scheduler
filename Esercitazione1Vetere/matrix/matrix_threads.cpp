/* matrix_threads.c */

#include <vector> 
#include <iostream>
#include <sstream>
#include <chrono>
#include <thread>

#include "matrix.h"

/* dimensione standard della matrice */
const size_t STD_SIZE = 10;

/* dimensione max per stampare le matrici a video */
const size_t MAX_PRINT_SIZE = 30;

/* Questa funzione moltiplica una riga di M1 per una colonna di M2
   posizionando il risultato in Res[row][column]
*/
void mult(size_t row, size_t column, const matrix & M1, const matrix & M2, matrix & Res)
{
	Res(row,column) = 0;

	for(size_t position = 0; position < Res.size(); ++position)
	{
		Res(row,column) += M1(row,position) * M2(position,column);
	}
}

void mult_row(size_t i, const matrix& A, const matrix& B, matrix& C) {
	for (size_t j = 0; j < A.size(); ++j)
		mult(i, j, A, B, C);
}

int main(int argc, char * argv[])
{
	size_t size = STD_SIZE;

	if (argc > 1)
	{
		std::istringstream iss(argv[1]);
		iss >> size;
	}

	matrix MA(size), MB(size), MC(size);

	std::vector<std::thread> th_vect;

	/* Riempimento delle matrice MA e MB: valori di esempio */
	for (size_t i = 0; i < size; ++i)
		for (size_t j = 0; j < size; ++j)
		{
			MA(i,j) = 1;
			MB(i,j) = i + j + 1;
		}

	if (size < MAX_PRINT_SIZE)
	{
		std::cout << "Matrix Ma:" << std::endl << MA;
		std::cout << "Matrix Mb:" << std::endl << MB;
	}

	auto start = std::chrono::high_resolution_clock::now();

	/* Calcolo del prodotto .............................. */
	for(size_t i = 0; i < size; ++i) {
		// crare i thread prende molto tempo!
		std::thread th(mult_row, i, std::cref(MA), std::cref(MB), std::ref(MC));
		th_vect.push_back(std::move(th));
		// th.join(); // non ho parallelizzato nulla! ho dei thread che eseguono uno dopo l'altro

		// th_vect.emplace_back(mult_row, i, std::cref(MA), std::cref(MB), std::ref(MC));
	}

	for(size_t i = 0; i < size; ++i) {
		th_vect[i].join();
	}

	auto stop = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double, std::milli> elapsed(stop - start);
	std::cout << "Elapsed [ms]: " << elapsed.count() << std::endl;
	/* ................................................... */

	if (size < MAX_PRINT_SIZE)
	{
		std::cout << "Matrix Mc (Ma + Mb):" << std::endl << MC;
	}

	return 0;
}

