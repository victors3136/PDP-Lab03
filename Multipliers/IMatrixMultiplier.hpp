//
// Created by victo on 28/10/2024.
//

#ifndef LAB3_SIMPLY_PARALLEL_IMATRIXMULTIPLIER_HPP
#define LAB3_SIMPLY_PARALLEL_IMATRIXMULTIPLIER_HPP

#include "../Domain/Matrix.hpp"
#include <cassert>
#include <functional>
#include <thread>
#include <optional>
#include "../Domain/Position.hpp"

#define VIRTUAL_CORE_COUNT 4

template<size_t size>
class IMatrixMultiplier {
private:
    const Matrix<size> leftOperand;
    const Matrix<size> rightOperand;

protected:
    Matrix<size> result;
    bool isReady = false;

    void computeCell(Position pos) {
        assert(pos.row < size);
        assert(pos.column < size);
        const auto rowId = pos.row;
        const auto colId = pos.column;
        result[rowId][colId] = 0;
        for (auto current = 0; current < size; ++current) {
            result[rowId][colId] += leftOperand[rowId][current] * rightOperand[current][colId];
        }
    }

public:
    explicit IMatrixMultiplier(const char *leftFilename, const char *rightFilename) :
            leftOperand{fromFile<size>(leftFilename)},
            rightOperand{fromFile<size>(rightFilename)},
            result{} {}

    const Matrix<size> &getResult() const {
        return result;
    }

    [[nodiscard]] virtual bool isResultReady() const { return isReady; };

    virtual void operator()() = 0;
};


#endif //LAB3_SIMPLY_PARALLEL_IMATRIXMULTIPLIER_HPP
