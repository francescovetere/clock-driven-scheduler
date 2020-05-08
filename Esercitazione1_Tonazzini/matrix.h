#ifndef MY_MATRIX
#define MY_MATRIX

#include <vector>
#include <iostream>
#include <iomanip>

/* matrice quadrata, di interi*/
class matrix
{
    public:
        matrix(size_t mat_size) : buffer(mat_size * mat_size, 0), dim(mat_size)
        {
        }

        int & operator()(size_t i, size_t j)
        {
            return buffer[(int)i * dim + j];
        }

        int const & operator()(size_t i, size_t j) const
        {
            return buffer[i * dim + j];
        }

        size_t size() const
        {
            return dim;
        }

        void print(std::ostream& os) const
        {
	        for(size_t i = 0; i < dim; ++i)
	        {
		        for (size_t j = 0; j < dim; ++j)
			        os << std::setw(5) << (*this)(i,j);

		        os << std::endl;
	        }
        } 

    private:
        std::vector<int> buffer;
        size_t dim;
};

std::ostream& operator <<(std::ostream& stream, const matrix & m)
{
	m.print(stream);
	return stream;
}

#endif
