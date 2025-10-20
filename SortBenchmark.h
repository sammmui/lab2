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
    static long long measureTime