#include <chrono>
#include <cstdlib>
#include <iostream>
#include <numeric>
#include <thread>
#include <vector>

void calculate_partial_sum(const std::vector<int>& arr, std::size_t start, std::size_t end, long long& partial_sum)
{
    partial_sum = std::accumulate(arr.begin() + start, arr.begin() + end, 0LL);
}

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        std::cerr << "Usage: " << argv[0] << " <array_size> <thread_count>" << std::endl;
        return 1;
    }

    std::size_t N = std::stoull(argv[1]);
    int M = std::stoi(argv[2]);

    std::vector<int> arr(N);
    for (auto& element : arr)
    {
        element = rand() % 100;
    }

    auto start_time = std::chrono::high_resolution_clock::now();
    long long single_thread_sum = std::accumulate(arr.begin(), arr.end(), 0LL);
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> single_thread_duration = end_time - start_time;
    std::cout << "Time spent without threads: " << single_thread_duration.count() << std::endl;

    start_time = std::chrono::high_resolution_clock::now();
    std::vector<std::thread> threads;
    std::vector<long long> partial_sums(M, 0);
    std::size_t chunk_size = N / M;

    for (int i = 0; i < M; ++i)
    {
        std::size_t start = i * chunk_size;
        std::size_t end = (i == M - 1) ? N : start + chunk_size;
        threads.emplace_back(calculate_partial_sum, std::cref(arr), start, end, std::ref(partial_sums[i]));
    }

    for (auto& t : threads)
    {
        t.join();
    }

    long long multi_thread_sum = std::accumulate(partial_sums.begin(), partial_sums.end(), 0LL);
    end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> multi_thread_duration = end_time - start_time;

    std::cout << "Time spent with " << M << " threads: " << multi_thread_duration.count() << std::endl;

    return 0;
}