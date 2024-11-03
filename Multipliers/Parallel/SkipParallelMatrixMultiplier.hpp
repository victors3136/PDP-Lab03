//
// Created by victo on 01/11/2024.
//

#ifndef LAB3_SIMPLY_PARALLEL_SKIPPARALLELMATRIXMULTIPLIER_HPP
#define LAB3_SIMPLY_PARALLEL_SKIPPARALLELMATRIXMULTIPLIER_HPP

#include "../IParallelMatrixMultiplier.hpp"
#include <barrier>
#include <syncstream>
#include <iostream>

template<size_t size>
class SkipParallelMatrixMultiplier : public IParallelMatrixMultiplier<size> {
protected:
    void startThreads() override {
        const auto threadCount = this->threads.size();
        for (auto threadId = 0ull; threadId < threadCount; ++threadId) {
            this->threads.emplace_back([this, threadId, threadCount]() {
                this->barrier.arrive_and_wait();
                for (auto position = threadId; position < size * size; position += threadCount) {
                    this->computeCell({position / size, position % size});
                }
            });
        }
    }

public:
    SkipParallelMatrixMultiplier(const char *leftFilename,
                                 const char *rightFilename,
                                 size_t threadCount = VIRTUAL_CORE_COUNT) :
            IParallelMatrixMultiplier<size>{leftFilename, rightFilename, threadCount} {}
};

#endif //LAB3_SIMPLY_PARALLEL_SKIPPARALLELMATRIXMULTIPLIER_HPP
