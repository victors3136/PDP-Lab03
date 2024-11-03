#ifndef LAB3_SIMPLY_PARALLEL_MATRIX_HPP
#define LAB3_SIMPLY_PARALLEL_MATRIX_HPP

#include <array>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>

template<size_t size>
using Matrix = std::array<std::array<int, size>, size>;

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

template<size_t size>
Matrix<size> fromFile(const char *filename) {
    Matrix<size> matrix{};
    std::ifstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error(std::string("Could not open file ") + filename);
    }
    size_t row = 0;
    for (std::string line; std::getline(file, line) && row < size; ++row) {
        std::istringstream stream{line};
        for (size_t col = 0; col < size; ++col) {
            if (stream >> matrix[row][col]) { continue; }
            throw std::runtime_error(
                    std::string("Not enough columns for row ")
                    + std::to_string(row)
                    + " in file "
                    + filename);
        }
    }

    if (row != size) {
        throw std::runtime_error("File does not contain enough rows for a matrix of the specified size");
    }
    return matrix;
}

#endif //LAB3_SIMPLY_PARALLEL_MATRIX_HPP
