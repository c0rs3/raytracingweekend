#pragma once
#ifndef _BENCHMARK_
#define _BENCHMARK_
#include <iostream>
#include <chrono>
#define _BENCHMARK_BEGIN namespace benchmark {
#define _BENCHMARK_END }

_BENCHMARK_BEGIN
static size_t t_alloc_size;
static size_t t_alloc_count;

template<typename type>
class Benchmark {
private:
public:
    static size_t alloc_size;

	std::chrono::duration<type> duration;
    std::chrono::steady_clock::time_point start; 

    Benchmark(){
        start = std::chrono::high_resolution_clock::now();
    }

    ~Benchmark(){
        std::chrono::steady_clock::time_point end = std::chrono::high_resolution_clock::now();
        duration = end - start;
        std::clog << "Duration: " << std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() << "ms" << " || ";
        std::clog << "Duration: " << std::chrono::duration_cast<std::chrono::seconds>(duration).count() << "s" << std::endl;
    }
};


void log_heap_alloc(){
    std::clog << "Total (heap) allocation size: "<< t_alloc_size << " bytes" << std::endl;
    std::clog << "Total (heap) allocation count: "<< t_alloc_count << " bytes" << std::endl;
}

_BENCHMARK_END

void* operator new(size_t size) noexcept{
    // std::cout << "Allocated " << size << " bytes of memory" << std::endl;
    benchmark::t_alloc_size += size;
    benchmark::t_alloc_count += 1;
    return malloc(size);
}

void* operator new[](size_t size) noexcept{
    benchmark::t_alloc_size += size;
    benchmark::t_alloc_count += 1;
    return malloc(size);
}

void operator delete(void* memory) noexcept {
    // std::cout << "Freed " << sizeof(memory) << " bytes of memory" << std::endl;
    free(memory);
}

void operator delete[](void* memory) noexcept {
    // std::cout << "Freed " << sizeof(memory) << " bytes" << std::endl;
    free(memory);
}
#endif