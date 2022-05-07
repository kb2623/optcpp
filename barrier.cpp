#include "barrier.hpp"

void Barrier::wait() {
    std::unique_lock<std::mutex> lock{mMutex};
    if (!--mCount) {
        mCount = mTreshold;
        mCond.notify_all();
    } else {
        mCond.wait(lock);
    }
}
