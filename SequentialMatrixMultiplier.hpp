
#ifndef LAB3_SIMPLY_PARALLEL_SEQUENTIALMATRIXMULTIPLIER_HPP
#define LAB3_SIMPLY_PARALLEL_SEQUENTIALMATRIXMULTIPLIER_HPP

#include "Matrix.hpp"
#include "Types.hpp"

#include <cstdio>

template<size_t size>
class SequentialMatrixMultiplier {
private:
    const Matrix<size> leftMatrix;
    const Matrix<size> rightMatrix;
    mutable Matrix<size> resultMatrix{};

    void computeCell(size_t idxRow, size_t idxCol) const {
        assert(idxRow < size && "Invalid row index");
        assert(idxCol < size && "Invalid col index");
        int sum = 0;
        for (int index = 0; index < size; ++index) {
            sum += leftMatrix[idxRow][index] * rightMatrix[index][idxCol];
        }
        resultMatrix[idxRow][idxCol] = sum;
    }
public:
    static SequentialMatrixMultiplier fromFile(std::ifstream &input) {
        Matrix<size> m1{}, m2{};
        for (size_t row = 0; row < size; ++row) {
            for (size_t col = 0; col < size; ++col) {
                if (!(input >> m1[row][col])) {
                    throw std::runtime_error("Error reading matrix from file, not enough numbers.");
                }
                if (!(input >> m2[row][col])) {
                    throw std::runtime_error("Error reading matrix from file, not enough numbers.");
                }

            }
        }
        return SequentialMatrixMultiplier<size>{std::move(m1), std::move(m2)};
    }
    SequentialMatrixMultiplier(
            Matrix<size> &&leftMatrix,
            Matrix<size> &&rightMatrix)
            : leftMatrix{leftMatrix},
              rightMatrix{rightMatrix} {
        for(auto row = 0 ; row < size; ++row){
            for(auto col = 0 ; col < size; ++col){
                computeCell(row, col);
            }
        }
    }

    bool check(const ParallelMatrixMultiplier<size> &subject) {
        if (leftMatrix != subject.leftMatrix) {
            std::cerr << "LHS input mismatch" << std::endl;
            return false;
        }
        if (rightMatrix != subject.rightMatrix) {
            std::cerr << "RHS input mismatch" << std::endl;
            return false;
        }
        if (resultMatrix != subject.resultMatrix) {
            std::cerr << "Result mismatch" << std::endl;
            return false;
        }
        return true;
    }
};

#endif //LAB3_SIMPLY_PARALLEL_SEQUENTIALMATRIXMULTIPLIER_HPP
