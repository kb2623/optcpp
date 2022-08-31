#ifndef _THREAD_DATA_H_
#define _THREAD_DATA_H_

#include <cstddef>
#include <random>

class thread_data {
public:
	thread_data();
	thread_data(size_t tid);
	thread_data(size_t tid, size_t seed);
	thread_data(size_t tid, std::default_random_engine prand, std::uniform_int_distribution<size_t> dist);
	thread_data(const thread_data& o);
	~thread_data() = default;

	size_t rand();
	double randDouble();

	const size_t tid;
	std::default_random_engine prand;
	std::uniform_int_distribution<size_t> dist;
};

thread_local thread_data* thread_td = nullptr;


#endif
