#include "thread_data.hpp"

#include <limits>

thread_data::thread_data() : tid(0), prand(std::default_random_engine()), dist(std::uniform_int_distribution<size_t>(0, std::numeric_limits<size_t>::max())) {}

thread_data::thread_data(size_t tid) : tid(tid), prand(std::default_random_engine()), dist(std::uniform_int_distribution<size_t>(0, std::numeric_limits<size_t>::max())) {}

thread_data::thread_data(size_t tid, size_t seed) : tid(tid), prand(std::default_random_engine(seed)), dist(std::uniform_int_distribution<size_t>(0, std::numeric_limits<size_t>::max())) {}

thread_data::thread_data(const thread_data& o) : tid(o.tid), prand(o.prand), dist(o.dist) {}

thread_data::~thread_data() {}

size_t thread_data::rand() {
	return dist(prand);
}

double thread_data::randDouble() {
	auto r = rand();
	if (r == 0) return 0;
	else return r / double(std::numeric_limits<size_t>::max());
}
