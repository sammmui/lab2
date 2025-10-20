#include "analyzer.h"

analyzer::analyzer() {}
std::vector<int> analyzer::generate_data(size_t n) {
    std::vector<int> v(n);
      std::mt19937 gen(std::random_device{}());
     std::uniform_int_distribution<int> dist(0, 1000);
    for (auto& x : v) x = dist(gen);
    return v;
 }
 bool analyzer::predicate(int x) {
      return x <= 900;
}


template<typename Func>
double analyzer::measure_time(Func f) {
    auto start = std::chrono::high_resolution_clock::now();
    f();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;
    return diff.count();
}
void analyzer::test_library_algorithms(const std::vector<int>& data) {
    std::cout << "\n=== library algorithms all_of ===\n";
    std::cout << std::setw(25) << "policy" << std::setw(15) << "yime(second)" << "\n";