//
// Created by victo on 01/11/2024.
//

#ifndef LAB3_SIMPLY_PARALLEL_IPARALLELMATRIXMULTIPLIER_HPP
#define LAB3_SIMPLY_PARALLEL_IPARALLELMATRIXMULTIPLIER_HPP

#include "IMatrixMultiplier.hpp"
#include <barrier>

template<size_t size>
class IParallelMatrixMultiplier : public IMatrixMultiplier<size> {
protected:
    std::vector<std::thread> threads;
    std::barrier<> barrier;

    virtual void startThreads() = 0;

    size_t threadCount;
public:
    IParallelMatrixMultiplier(const char *leftFilename,
                              const char *rightFilename,
                              size_t threadCount = VIRTUAL_CORE_COUNT) :
            IMatrixMultiplier<size>(leftFilename, rightFilename),
            threadCount{threadCount},
            threads{threadCount},
            barrier(threadCount) {}

    void operator()() override {
        startThreads();
        for (auto &thread: threads) {
            if (thread.joinable()) {
                thread.join();
            }
        }
        this->isReady = true;
    }
};

#endif //LAB3_SIMPLY_PARALLEL_IPARALLELMATRIXMULTIPLIER_HPP
