#ifndef LAB3_SIMPLY_PARALLEL_TYPES_HPP
#define LAB3_SIMPLY_PARALLEL_TYPES_HPP

#include <array>
#include <cstdio>

template<size_t size>
using Matrix = std::array<std::array<int, size>, size>;

enum CellSelectionStrategy {
    SKIP,
    CONT_ROW,
    CONT_COL
};

template<size_t size>
class ParallelMatrixMultiplier;

template<size_t size>
class SequentialMatrixMultiplier;
#endif //LAB3_SIMPLY_PARALLEL_TYPES_HPP
