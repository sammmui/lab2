#pragma once
#include <vector>
#include <string>

class SortBenchmark {
private:
    std::vector<int> data;

public:
 SortBenchmark(size_t n);
 void runAllTests();
 static std::vector<int> generateData(size_t n);

   template <typename Func>
static long long measureTime(Func func);

    static void parallelSort(std::vector<int>& data, int threadCount);
 void testStdSort();
    void testExecutionSort();
    void testCustomParallelSort();
};