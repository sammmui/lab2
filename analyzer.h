#pragma once
#include <vector>
#include <random>
#include <iostream>
#include <iomanip>

class analyzer {
public:
    analyzer();
    void run_tests();

private:
std::vector<int> generate_data(size_t n);
    bool predicate(int x);
    void test_library_algorithms(const std::vector<int>& data);
       void test_parallel_algorithm(const std::vector<int>& data);
    bool custom_parallel_all_of(const std::vector<int>& data, size_t threads);

    template<typename Func>
    double measure_time(Func f);
};