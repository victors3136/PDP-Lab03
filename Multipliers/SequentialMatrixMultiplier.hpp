//
// Created by victo on 01/11/2024.
//

#ifndef LAB3_SIMPLY_PARALLEL_SEQUENTIALMATRIXMULTIPLIER_HPP
#define LAB3_SIMPLY_PARALLEL_SEQUENTIALMATRIXMULTIPLIER_HPP

#include "IMatrixMultiplier.hpp"

template<size_t size>
class SequentialMatrixMultiplier : public IMatrixMultiplier<size> {
public:
    SequentialMatrixMultiplier(const char *leftFilename, const char *rightFilename) :
            IMatrixMultiplier<size>(leftFilename, rightFilename) {

    }

    void operator()() override {
        for (auto row = 0ull; row < size; ++row) {
            for (auto col = 0ull; col < size; ++col) {
                this->computeCell({row, col});
            }
        }
        this->isReady = true;
    }
};

#endif //LAB3_SIMPLY_PARALLEL_SEQUENTIALMATRIXMULTIPLIER_HPP
