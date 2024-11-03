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
//  Have a function that computes a single element of the resulting matrix.
//
//  Have a second function that, will constitute a parallel task on each call (that is,
//  this function will be called on several threads in parallel).
//  This function will call the above one several times consecutively to compute several
//  elements of the resulting matrix. Consider the following ways of splitting the work
//  between tasks (for the examples, consider the final matrix being 9x9 and the work split into 4 tasks):
//
//  Each task computes consecutive elements, going row after row.
//  So, task 0 computes rows 0 and 1, plus elements 0-1 of row 2 (20 elements in total);
//  task 1 computes the remainder of row 2, row 3, and elements 0-3 of row 4 (20 elements);
//  task 2 computes the remainder of row 4, row 5, and elements 0-5 of row 6 (20 elements);
//  finally, task 3 computes the remaining elements (21 elements).
//
// Each task computes consecutive elements, going column after column.
// This is like the previous example, but interchanging the rows with the columns:
// task 0 takes columns 0 and 1, plus elements 0 and 1 from column 2, and so on.
//
// Each task takes every k-th element (where k is the number of tasks), going row by row.
// So, task 0 takes elements (0,0), (0,4), (0,8), (1,3), (1,7), (2,2), (2,6), (3,1), (3,5), (4,0), etc.
//
// For running the tasks, also implement 2 approaches:
//
// Create an actual thread for each task (use the low-level thread mechanism from the programming language);
// Use a thread pool.
//
// Experiment with various values for (and document the attempts and their performance):
//
// The sizes of the matrix;
// The number of tasks (this is equal to the number of threads when not using a thread pool);
// The number of threads and other parameters for the thread pool (when using the thread pool).

#include "Domain/Matrix.hpp"
#include "Domain/Constants.hpp"
#include "Multipliers/SequentialMatrixMultiplier.hpp"
#include "Multipliers/Parallel/SkipParallelMatrixMultiplier.hpp"
#include "Multipliers/Parallel/RowParallelMatrixMultiplier.hpp"
#include "Multipliers/Parallel/ColumnParallelMatrixMultiplier.hpp"
#include "Multipliers/Pooled/SkipPooledMatrixMultiplier.hpp"
#include "Multipliers/Pooled/RowPooledMatrixMultiplier.hpp"
#include "Multipliers/Pooled/ColumnPooledMatrixMultiplier.hpp"

#include <cassert>
#include <fstream>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-msc50-cpp"

void writeInputs() {
    srand(std::chrono::system_clock::now().time_since_epoch().count());
    std::ofstream finL{"left.in"};
    for (int i = 0; i < 1000; ++i) {
        for (int j = 0; j < 1000; ++j) {
            finL << rand() % 10 << " ";
        }
        finL << std::endl;
    }
    finL.close();
    std::ofstream finR{"right.in"};
    for (int i = 0; i < 1000; ++i) {
        for (int j = 0; j < 1000; ++j) {
            finR << rand() % 10 << " ";
        }
        finR << std::endl;
    }
    finR.close();
}

#pragma clang diagnostic pop

template<size_t size>
void measure(SequentialMatrixMultiplier<size> *evaluator, IMatrixMultiplier<size> *evaluated,
             const char *message) {
    assert(evaluator->isResultReady());
    assert(!evaluated->isResultReady());
    const auto startTime = std::chrono::system_clock::now();
    (*evaluated)();
    while (!evaluated->isResultReady());
    const auto endTime = std::chrono::system_clock::now();
    assert(evaluator->getResult() == evaluated->getResult());
    std::cout
            << "[" << duration_cast<std::chrono::milliseconds>((endTime - startTime)).count()
            << " milliseconds] " << message
            << std::endl;
}

int main() {
#pragma clang diagnostic push
#pragma ide diagnostic ignored "Simplify"
    if constexpr (WRITE_INPUTS) {
        writeInputs();
        return 0;
    }
#pragma clang diagnostic pop
    std::cout << "Matrix size: " << MATRIX_SIZE
              << "\nParallel thread count: " << PARALLEL_THREAD_COUNT
              << "\nPool size: " << POOL_SIZE << std::endl;
    SequentialMatrixMultiplier<MATRIX_SIZE> sequential{"left.in", "right.in"};


    sequential();
    while (!sequential.isResultReady());
    std::cout << "Starting ..." << std::endl;

    {
        SkipParallelMatrixMultiplier<MATRIX_SIZE> skipParallel{"left.in", "right.in", PARALLEL_THREAD_COUNT};
        measure(&sequential, &skipParallel, "Skip Parallel");
    }
    {
        RowParallelMatrixMultiplier<MATRIX_SIZE> rowParallel{"left.in", "right.in", PARALLEL_THREAD_COUNT};
        measure(&sequential, &rowParallel, "Row Parallel");
    }
    {
        ColumnParallelMatrixMultiplier<MATRIX_SIZE> columnParallel{"left.in", "right.in", PARALLEL_THREAD_COUNT};
        measure(&sequential, &columnParallel, "Column Parallel");
    }
    {
        SkipPooledMatrixMultiplier<MATRIX_SIZE> skipPooled{"left.in", "right.in", POOL_SIZE};
        measure(&sequential, &skipPooled, "Skip Pooled");
    }
    {
        RowPooledMatrixMultiplier<MATRIX_SIZE> rowPooled{"left.in", "right.in", POOL_SIZE};
        measure(&sequential, &rowPooled, "Row Pooled");
    }
    {
        ColumnPooledMatrixMultiplier<MATRIX_SIZE> columnPooled{"left.in", "right.in", POOL_SIZE};
        measure(&sequential, &columnPooled, "Column Pooled");
    }
    std::cout << "All multiplications completed successfully" << std::endl;
    return 0;
}
