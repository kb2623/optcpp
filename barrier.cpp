#include "barrier.hpp"

#if __cplusplus >= 202002L


#else

void Barrier::arrive_and_wait() {
	std::unique_lock<std::mutex> lock{mMutex};
	auto inst = instance;
	if (!--mCount) {
		mCount = mTreshold;
		instance++;
		mCond.notify_all();
	} else {
		mCond.wait(lock, [this, &inst] { return instance != inst; });
	}
}

#endif
