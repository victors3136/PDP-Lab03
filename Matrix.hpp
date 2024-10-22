#ifndef LAB3_SIMPLY_PARALLEL_MATRIX_HPP
#define LAB3_SIMPLY_PARALLEL_MATRIX_HPP

#include "Types.hpp"

#include <array>
#include <cstdlib>
#include <iostream>

template<size_t size>
bool operator==(const Matrix<size> &one, const Matrix<size> &other) {
    for (int row = 0; row < size; ++row) {
        for (int col = 0; col < size; ++col) {
            if (one[row][col] != other[row][col]) {
                std::cout << "failed on [" << row << ", " << col << "]" << std::endl;
                std::cout << "with " << one[row][col] << ", " << other[row][col] << std::endl;
                return false;
            }
        }
    }
    return true;
}


#endif //LAB3_SIMPLY_PARALLEL_MATRIX_HPP
