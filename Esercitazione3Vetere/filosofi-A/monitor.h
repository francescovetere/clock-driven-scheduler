#ifndef MONITOR_H
#define MONITOR_H

#include <vector>
#include <mutex>
#include <condition_variable>

class PhiMonitor
{
	public:
		PhiMonitor(size_t num);

		void pickup(size_t phi_id);
		void putdown(size_t phi_id);

		size_t size() const;

	private:
		enum phi_state {THINKING, HUNGRY, EATING};

		std::mutex mutex;
		std::condition_variable cond;
		std::vector<phi_state> state;

		size_t left(size_t id) const;  // id of the philosopher on the left side
		size_t right(size_t id) const; // id of the philosopher on the right side
};

inline size_t PhiMonitor::size() const
{
	return state.size();
}

#endif
