#ifndef LAB3_SIMPLY_PARALLEL_PARALLELMATRIXMULTIPLIER_HPP
#define LAB3_SIMPLY_PARALLEL_PARALLELMATRIXMULTIPLIER_HPP

#include "Matrix.hpp"
#include "Types.hpp"

#include <cassert>
#include <functional>
#include <thread>
#include <vector>


template<size_t size>
class ParallelMatrixMultiplier {
private:
    static constexpr auto absoluteUpperBound = size * size;
    const size_t threadCount;
    const Matrix<size> leftMatrix;
    const Matrix<size> rightMatrix;
    mutable Matrix<size> resultMatrix{};
    std::vector<std::thread> threads{};
    size_t cellsToComputePerThread;

    friend class SequentialMatrixMultiplier<size>;

public:
    static ParallelMatrixMultiplier fromFile(
            std::ifstream &input,
            size_t threadCount = 4,
            CellSelectionStrategy strategy = CONT_ROW) {
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
        return ParallelMatrixMultiplier<size>{std::move(m1), std::move(m2), threadCount, strategy};
    }

    ParallelMatrixMultiplier(
            Matrix<size> &&leftMatrix,
            Matrix<size> &&rightMatrix,
            size_t threadCount = 4,
            CellSelectionStrategy strategy = SKIP)
            : threadCount{threadCount},
              leftMatrix{leftMatrix},
              rightMatrix{rightMatrix} {
        assert(threadCount > 0 && "Cannot have thread count == 0");
        cellsToComputePerThread = (size * size) / threadCount;
        threads.reserve(threadCount);
        for (auto threadId = 0; threadId < threadCount; ++threadId) {
            threads.emplace_back(getAppropriateStrategy(strategy, threadId));
        }
    }


    void collect() {
        for (auto &thread: threads) {
            if (thread.joinable()) { thread.join(); }
        }
    }

private:
    void computeCell(size_t idxRow, size_t idxCol) const {
        assert(idxRow < size && "Invalid row index");
        assert(idxCol < size && "Invalid col index");
        int sum = 0;
        for (int index = 0; index < size; ++index) {
            sum += leftMatrix[idxRow][index] * rightMatrix[index][idxCol];
        }
        resultMatrix[idxRow][idxCol] = sum;
    }

    std::function<void()> threadBodyWithSkipStrategy(size_t index) const {
        return [this, index] {
            for (auto current = index; current < absoluteUpperBound; current += threadCount) {
                computeCell(current / size, current % size);
            }
        };
    }

    std::function<void()> threadBodyWithContinuousRowStrategy(size_t index) const {
        const auto lowerBound = index * cellsToComputePerThread;
        const auto generalUpperBound = (index + 1) * cellsToComputePerThread;
        // In the case when there are only a few cells left, give them out to the last thread to handle
        const auto upperBound = generalUpperBound < absoluteUpperBound
                                ? generalUpperBound : absoluteUpperBound;
        return [this, lowerBound, upperBound] {
            for (auto current = lowerBound; current < upperBound; ++current) {
                computeCell(current / size, current % size);
            }
        };
    }

    std::function<void()> threadBodyWithContinuousColStrategy(size_t index) const {
        const auto lowerBound = index * cellsToComputePerThread;
        const auto generalUpperBound = (index + 1) * cellsToComputePerThread;
        // In the case when there are only a few cells left, give them out to the last thread to handle
        const auto upperBound = generalUpperBound < absoluteUpperBound
                                ? generalUpperBound : absoluteUpperBound;
        return [this, lowerBound, upperBound] {
            for (auto current = lowerBound; current < upperBound; ++current) {
                computeCell(current % size, current / size);
            }
        };
    }

    auto getAppropriateStrategy(CellSelectionStrategy strategy, size_t index) const {
        switch (strategy) {
            case SKIP:
                return threadBodyWithSkipStrategy(index);
            case CONT_ROW:
                return threadBodyWithContinuousRowStrategy(index);
            case CONT_COL:
                return threadBodyWithContinuousColStrategy(index);
            default:
                throw std::runtime_error{"Invalid strategy"};
        }
    }

};

#endif //LAB3_SIMPLY_PARALLEL_PARALLELMATRIXMULTIPLIER_HPP