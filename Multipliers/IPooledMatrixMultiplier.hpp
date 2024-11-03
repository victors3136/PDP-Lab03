//
// Created by victo on 01/11/2024.
//

#ifndef LAB3_SIMPLY_PARALLEL_IPOOLEDMATRIXMULTIPLIER_HPP
#define LAB3_SIMPLY_PARALLEL_IPOOLEDMATRIXMULTIPLIER_HPP

#include "../Domain/SynchronizedTaskQueue.hpp"
#include "IParallelMatrixMultiplier.hpp"

template<size_t size>
class IPooledMatrixMultiplier : public IParallelMatrixMultiplier<size> {
protected:
    SynchronizedTaskQueue queue;

    void startThreads() override {
        for (auto index = 0ull; index < this->threadCount; ++index) {
            this->threads.emplace_back([this]() {
                this->barrier.arrive_and_wait();
                while (true) {
                    const auto task = queue.pop();
                    if (!task) break;
                    (*task)();
                }
            });
        }
    }

    virtual void populateQueue() = 0;

public:

    IPooledMatrixMultiplier(const char *leftFilename,
                            const char *rightFilename,
                            size_t threadCount = VIRTUAL_CORE_COUNT) :
            IParallelMatrixMultiplier<size>(leftFilename, rightFilename, threadCount) {}

    void operator()() override {
        populateQueue();
        startThreads();
        for (auto &thread: this->threads) {
            if (thread.joinable()) {
                thread.join();
            }
        }
        this->isReady = true;
    }
};

#endif //LAB3_SIMPLY_PARALLEL_IPOOLEDMATRIXMULTIPLIER_HPP
