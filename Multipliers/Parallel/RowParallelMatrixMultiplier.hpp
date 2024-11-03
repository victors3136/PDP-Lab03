//
// Created by victo on 01/11/2024.
//

#ifndef LAB3_SIMPLY_PARALLEL_ROWPARALLELMATRIXMULTIPLIER_HPP
#define LAB3_SIMPLY_PARALLEL_ROWPARALLELMATRIXMULTIPLIER_HPP

#include "../IParallelMatrixMultiplier.hpp"
#include <barrier>
#include <syncstream>
#include <iostream>

template<size_t size>
class RowParallelMatrixMultiplier : public IParallelMatrixMultiplier<size> {
protected:
    void startThreads() override {
        const auto threadCount = this->threads.size();
        const auto cellsPerThread = (size * size) / threadCount;
        const auto remainder = (size * size) % threadCount;

        for (auto threadId = 0ull; threadId < threadCount; ++threadId) {
            const auto startCell = threadId * cellsPerThread + std::min(threadId, remainder);
            const auto endCell = startCell + cellsPerThread + (threadId < remainder ? 1 : 0);

            this->threads.emplace_back([this, startCell, endCell]() {
                this->barrier.arrive_and_wait();
                for (auto cell = startCell; cell < endCell; ++cell) {
                    this->computeCell({cell / size, cell % size});
                }
            });
        }
    }

public:
    RowParallelMatrixMultiplier(const char *leftFilename,
                                const char *rightFilename,
                                size_t threadCount = VIRTUAL_CORE_COUNT) :
            IParallelMatrixMultiplier<size>{leftFilename, rightFilename, threadCount} {}
};

#endif //LAB3_SIMPLY_PARALLEL_ROWPARALLELMATRIXMULTIPLIER_HPP
