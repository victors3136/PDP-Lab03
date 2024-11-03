//
// Created by victo on 01/11/2024.
//

#ifndef LAB3_SIMPLY_PARALLEL_SKIPPOOLEDMATRIXMULTIPLIER_HPP
#define LAB3_SIMPLY_PARALLEL_SKIPPOOLEDMATRIXMULTIPLIER_HPP

#include "../IPooledMatrixMultiplier.hpp"

template<size_t size>
class SkipPooledMatrixMultiplier : public IPooledMatrixMultiplier<size> {
protected:
    void populateQueue() override {
        for (auto threadId = 0ull; threadId < this->threadCount; ++threadId) {
            for (auto position = threadId; position < size * size; position += this->threadCount) {
                this->queue.put([this, position] {
                    this->computeCell({position / size, position % size});
                });
            }
        }
    }

public:
    SkipPooledMatrixMultiplier(
            const char *leftFilename,
            const char *rightFilename,
            size_t threadCount = VIRTUAL_CORE_COUNT) :
            IPooledMatrixMultiplier<size>(leftFilename, rightFilename, threadCount) {}
};

#endif //LAB3_SIMPLY_PARALLEL_SKIPPOOLEDMATRIXMULTIPLIER_HPP
