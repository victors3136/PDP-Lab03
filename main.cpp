#include "ParallelMatrixMultiplier.hpp"
#include "SequentialMatrixMultiplier.hpp"

#include <cassert>
#include <fstream>
#include <iostream>

// Lab 3 - Simple parallel tasks
//
// Goal
//  Divide a simple task between threads.
//  The task can easily be divided in sub-tasks requiring no cooperation at all.
//  See the caching effects, and the costs of creating threads and of switching between threads.
//
// Requirement
//  Write several programs to compute the product of two matrices.
//
//  Have a function that computes a single element of the resulting matrix. ✅
//
//  Have a second function that, will constitute a parallel task on each call (that is,
//  this function will be called on several threads in parallel). ✅
//  This function will call the above one several times consecutively to compute several
//  elements of the resulting matrix. Consider the following ways of splitting the work
//  between tasks (for the examples, consider the final matrix being 9x9 and the work split into 4 tasks):
//
//  Each task computes consecutive elements, going row after row. ✅
//  So, task 0 computes rows 0 and 1, plus elements 0-1 of row 2 (20 elements in total);
//  task 1 computes the remainder of row 2, row 3, and elements 0-3 of row 4 (20 elements);
//  task 2 computes the remainder of row 4, row 5, and elements 0-5 of row 6 (20 elements);
//  finally, task 3 computes the remaining elements (21 elements).
//
// Each task computes consecutive elements, going column after column. ✅
// This is like the previous example, but interchanging the rows with the columns:
// task 0 takes columns 0 and 1, plus elements 0 and 1 from column 2, and so on.
//
// Each task takes every k-th element (where k is the number of tasks), going row by row. ✅
// So, task 0 takes elements (0,0), (0,4), (0,8), (1,3), (1,7), (2,2), (2,6), (3,1), (3,5), (4,0), etc.
//
// For running the tasks, also implement 2 approaches:
//
// Create an actual thread for each task (use the low-level thread mechanism from the programming language); ✅
// Use a thread pool.
//
// Experiment with various values for (and document the attempts and their performance):
//
// The sizes of the matrix;
// The number of tasks (this is equal to the number of threads when not using a thread pool);
// The number of threads and other parameters for the thread pool (when using the thread pool).


int main() {
    constexpr auto matrixSize = 43;
    constexpr auto threadCount = 7;
    std::ifstream
            finSkip{"inputSkip.txt"},
            finContRow{"inputContRow.txt"},
            finContCol{"inputContCol.txt"},
            finCheck{"inputCheck.txt"};
    if (!finSkip.is_open() || !finContRow.is_open() || !finContCol.is_open() || !finCheck.is_open()) {
        std::cerr << "Could not open :(" << std::endl;
        return 1;
    }
    auto smmControl = SequentialMatrixMultiplier<matrixSize>::fromFile(finCheck);
    auto pmmContRow = ParallelMatrixMultiplier<matrixSize>::fromFile(
            finContRow,
            threadCount,
            CellSelectionStrategy::CONT_ROW);
    auto pmmSkip = ParallelMatrixMultiplier<matrixSize>::fromFile(
            finSkip,
            threadCount,
            CellSelectionStrategy::SKIP);
    auto pmmContCol = ParallelMatrixMultiplier<matrixSize>::fromFile(
            finSkip,
            threadCount,
            CellSelectionStrategy::CONT_COL);
    finContRow.close();
    finContCol.close();
    finSkip.close();
    finCheck.close();
    pmmContRow.collect();
    pmmSkip.collect();
    pmmContCol.collect();
    assert(smmControl.check(pmmSkip) && "Skip strategy is wrong");
    assert(smmControl.check(pmmContRow) && "Cont row strategy is wrong");
    assert(smmControl.check(pmmContCol) && "Cont col strategy is wrong");

    return 0;
}
