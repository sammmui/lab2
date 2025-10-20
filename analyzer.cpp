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
    std::cout << std::setw(25) << "policy" << std::setw(15) << "time(second)" << "\n";
      double t1 = measure_time([&]() {
        std::all_of(data.begin(), data.end(), [&](int x) { return predicate(x); });
    });
    std::cout << std::setw(25) << "without policy" << std::setw(15) << t1 << "\n";

    double t2 = measure_time([&]() {
        std::all_of(std::execution::seq, data.begin(), data.end(), [&](int x) { return predicate(x); });
    });
    std::cout << std::setw(25) << "seq" << std::setw(15) << t2 << "\n";

    double t3 = measure_time([&]() {
        std::all_of(std::execution::par, data.begin(), data.end(), [&](int x) { return predicate(x); });
    });
    std::cout << std::setw(25) << "par" << std::setw(15) << t3 << "\n";

    double t4 = measure_time([&]() {
        std::all_of(std::execution::par_unseq, data.begin(), data.end(), [&](int x) { return predicate(x); });
    });
    std::cout << std::setw(25) << "par_unseq" << std::setw(15) << t4 << "\n";
}