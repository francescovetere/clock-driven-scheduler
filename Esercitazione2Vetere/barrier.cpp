#include "barrier.h"

void barrier::wait()
{
	std::unique_lock<std::mutex> lock(mutex);

	if (count > 0)
	{
		count--;

		if (count > 0)
		{
			while (count > 0)
				cond.wait(lock);
		}
		else
		{
			cond.notify_all();
		}
	}
}

