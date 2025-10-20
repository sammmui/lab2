#pragma once
#include <vector>
#include <string>

class analyzer {
public:
    analyzer();
    void run_tests();

private:
std::vector<int> generate_data(size_t n);
    bool predicate(int x);
    void test_library_algorithms(const std::vector<int>& data);