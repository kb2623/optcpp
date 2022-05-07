#ifndef _BARRIER_H_
#define _BARRIER_H_

#include <mutex>
#include <condition_variable>

class Barrier {
public:
    Barrier(size_t iCount) : mTreshold(iCount), mCount(iCount) {};

    void wait();

private:
    std::mutex mMutex;
    std::condition_variable mCond;
    size_t mTreshold;
    size_t mCount;
};

#endif
