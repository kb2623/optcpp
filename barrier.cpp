#include "barrier.hpp"

void Barrier::wait() {
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
