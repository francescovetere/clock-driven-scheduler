#ifndef BARRIER_H
#define BARRIER_H

#include <mutex>
#include <condition_variable>

class barrier
{
	public:
		barrier(unsigned int size);
		
		// Wait until "size" threads calls this method, then wake up everyone
		void wait();
		
		size_t size() const;

	private:
		std::mutex mutex;
		std::condition_variable cond;
	
		const unsigned int b_size;
		unsigned int count;
};

inline barrier::barrier(unsigned int size) : b_size(size), count(size)
{
}

inline size_t barrier::size() const
{
	return b_size;
}

#endif
