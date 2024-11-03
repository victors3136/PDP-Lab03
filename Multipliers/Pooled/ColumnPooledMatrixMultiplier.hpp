//
// Created by victo on 01/11/2024.
//

#ifndef LAB3_SIMPLY_PARALLEL_COLUMNPOOLEDMATRIXMULTIPLIER_HPP
#define LAB3_SIMPLY_PARALLEL_COLUMNPOOLEDMATRIXMULTIPLIER_HPP

#include "../IPooledMatrixMultiplier.hpp"

template<size_t size>
class ColumnPooledMatrixMultiplier : public IPooledMatrixMultiplier<size> {
protected:
    void populateQueue() override {
        for (auto col = 0ull; col < size; ++col) {
            for (auto row = 0ull; row < size; ++row) {
                this->queue.put([this, row, col]{this->computeCell({row, col});});
            }
        }
    }

public:
    ColumnPooledMatrixMultiplier(const char *leftFilename,
                              const char *rightFilename,
                              size_t threadCount = VIRTUAL_CORE_COUNT) :
            IPooledMatrixMultiplier<size>(leftFilename, rightFilename, threadCount) {}
};
#endif //LAB3_SIMPLY_PARALLEL_COLUMNPOOLEDMATRIXMULTIPLIER_HPP
