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

bool analyzer::custom_parallel_all_of(const std::vector<int>& data) {
    size_t n = data.size();
    size_t threads = std::thread::hardware_concurrency();
    if (threads == 0) threads = 2;

    size_t chunk = n / threads;
    std::vector<std::thread> workers;
    std::atomic<bool> result(true);

    for (size_t i = 0; i < threads; ++i) {
        size_t start = i * chunk;
        size_t end = (i == threads - 1) ? n : start + chunk;

        workers.emplace_back([&, start, end]() {
            for (size_t j = start; j < end && result; ++j) {
                if (!predicate(data[j])) {
                    result = false;
                    return;
                }
            }
        });
    }

    for (auto& t : workers) t.join();
    return result;
}

void analyzer::test_parallel_algorithm(const std::vector<int>& data) {
    std::cout << "\n=== own parallel all_of ===\n";
    size_t threads = std::thread::hardware_concurrency();
    if (threads == 0) threads = 2;

    std::cout << "number of threads: " << threads << "\n";

    double t = measure_time([&]() {
        custom_parallel_all_of(data);
    });

    std::cout << std::setw(25) << "own implementation" << std::setw(15) << t << "\n";
}

void analyzer::run_tests() {
    std::vector<size_t> sizes = {100000, 1000000, 10000000};

    for (auto n : sizes) {
        std::cout << "\n==============================\n";
        std::cout << "sample size: " << n << "\n";
        auto data = generate_data(n);

        test_library_algorithms(data);
        test_parallel_algorithm(data);
    }
}