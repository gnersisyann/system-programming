#include <pthread.h>

#include <chrono>
#include <cstdlib>
#include <iostream>
#include <numeric>
#include <vector>

struct ThreadData
{
    const std::vector<int>* arr;
    std::size_t start;
    std::size_t end;
    long long partial_sum;
};

void* calculate_partial_sum(void* arg)
{
    ThreadData* data = static_cast<ThreadData*>(arg);
    data->partial_sum = std::accumulate(data->arr->begin() + data->start, data->arr->begin() + data->end, 0LL);
    return nullptr;
}

int main(int argc, char* argv[])
{
    if (argc != 3)
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
    std::cout << "Time spent without threads: " << single_thread_duration.count() << " seconds" << std::endl;

    start_time = std::chrono::high_resolution_clock::now();
    std::vector<pthread_t> threads(M);
    std::vector<ThreadData> thread_data(M);
    std::size_t chunk_size = N / M;

    for (int i = 0; i < M; ++i)
    {
        thread_data[i] = {&arr, i * chunk_size, (i == M - 1) ? N : (i + 1) * chunk_size, 0};
        pthread_create(&threads[i], nullptr, calculate_partial_sum, &thread_data[i]);
    }

    for (int i = 0; i < M; ++i)
    {
        pthread_join(threads[i], nullptr);
    }

    long long multi_thread_sum = 0;
    for (const auto& data : thread_data)
    {
        multi_thread_sum += data.partial_sum;
    }

    end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> multi_thread_duration = end_time - start_time;
    std::cout << "Time spent with " << M << " threads: " << multi_thread_duration.count() << " seconds" << std::endl;

    return 0;
}