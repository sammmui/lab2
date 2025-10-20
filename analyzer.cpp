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
    std::cout << "Data size: " << data.size() << "\n";
    std::cout << std::setw(25) << "policy" 
              << std::setw(20) << "time (s)" 
              << std::setw(10) << "result" << "\n";
    std::cout << std::string(60, '-') << "\n";

    bool r1, r2, r3, r4;

    double t1 = measure_time([&]() {
        r1 = std::all_of(data.begin(), data.end(), [&](int x) { return predicate(x); });
    });
    std::cout << std::setw(25) << "no policy (std::all_of)" 
              << std::setw(20) << t1 
              << std::setw(10) << (r1 ? "true" : "false") << "\n";

    double t2 = measure_time([&]() {
        r2 = std::all_of(std::execution::seq, data.begin(), data.end(), [&](int x) { return predicate(x); });
    });
    std::cout << std::setw(25) << "seq" 
              << std::setw(20) << t2 
              << std::setw(10) << (r2 ? "true" : "false") << "\n";

    double t3 = measure_time([&]() {
        r3 = std::all_of(std::execution::par, data.begin(), data.end(), [&](int x) { return predicate(x); });
    });
    std::cout << std::setw(25) << "par" 
              << std::setw(20) << t3 
              << std::setw(10) << (r3 ? "true" : "false") << "\n";

    double t4 = measure_time([&]() {
        r4 = std::all_of(std::execution::par_unseq, data.begin(), data.end(), [&](int x) { return predicate(x); });
    });
    std::cout << std::setw(25) << "par_unseq" 
              << std::setw(20) << t4 
              << std::setw(10) << (r4 ? "true" : "false") << "\n";
}

bool analyzer::custom_parallel_all_of(const std::vector<int>& data, size_t threads) {
    size_t n = data.size();
    if (n == 0) return true;
    if (threads == 0) threads = 1;

    size_t actual_threads = std::min(threads, n);
    size_t chunk = n / actual_threads;

    std::vector<std::thread> workers;
    std::atomic<bool> result(true);

    for (size_t i = 0; i < actual_threads; ++i) {
        size_t start = i * chunk;
        size_t end = (i == actual_threads - 1) ? n : start + chunk;

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
    size_t max_threads = std::thread::hardware_concurrency();
    if (max_threads == 0) max_threads = 2;

    std::cout << std::setw(25) << "threads" 
              << std::setw(20) << "time (s)" 
              << std::setw(10) << "result" << "\n";
    std::cout << std::string(60, '-') << "\n";

    for (size_t k = 1; k <= max_threads * 2; k *= 2) {
        bool res = true;
        double t = measure_time([&]() {
            res = custom_parallel_all_of(data, k);
        });
        std::cout << std::setw(25) << k 
                  << std::setw(20) << t 
                  << std::setw(10) << (res ? "true" : "false") << "\n";
    }
}

void analyzer::run_tests() {
    std::vector<size_t> sizes = {100000, 1000000, 5000000};

    for (auto n : sizes) {
        std::cout << "\n==============================\n";
        std::cout << "sample size: " << n << "\n";
        auto data = generate_data(n);

        test_library_algorithms(data);
        test_parallel_algorithm(data);
    }
}
