//
// Created by victo on 01/11/2024.
//

#ifndef LAB3_SIMPLY_PARALLEL_ROWPOOLEDMATRIXMULTIPLIER_HPP
#define LAB3_SIMPLY_PARALLEL_ROWPOOLEDMATRIXMULTIPLIER_HPP

#include "../IPooledMatrixMultiplier.hpp"

template<size_t size>
class RowPooledMatrixMultiplier : public IPooledMatrixMultiplier<size> {
protected:
    void populateQueue() override {
        for (auto row = 0ull; row < size; ++row) {
            for (auto col = 0ull; col < size; ++col) {
                this->queue.put([this, row, col]{this->computeCell({row, col});});
            }
        }
    }

public:
    RowPooledMatrixMultiplier(const char *leftFilename,
                               const char *rightFilename,
                               size_t threadCount = VIRTUAL_CORE_COUNT) :
            IPooledMatrixMultiplier<size>(leftFilename, rightFilename, threadCount) {}
};

#endif //LAB3_SIMPLY_PARALLEL_ROWPOOLEDMATRIXMULTIPLIER_HPP
