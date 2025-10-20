#include "analyzer.h"

analyzer::analyzer() {}
std::vector<int> analyzer::generate_data(size_t n) {
    std::vector<int> v(n);
     std::uniform_int_distribution<int> dist(0, 1000);
    for (auto& x : v) x = dist(gen);
    return v;