#include <iostream>
#include <vector>
#include <chrono>
#include "headers/ewah.h"
#include "roaring.hh"

typedef EWAHBoolArray<uint64_t> bitmap;

void bench_ewah(int cardinality)
{
    bitmap x;
    for(auto i = 0; i <= 100000000; i++) {
        if(i % cardinality == 0) x.set(i);
    }

    auto counter = 0;
    auto start = std::chrono::steady_clock::now();
    for(auto i = x.begin() ; i != x.end() ; i++) ++counter;
    auto end = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "EWAH Entries : " << counter
        << " Time: " << elapsed.count()
        << " Size: " << x.sizeInBits() / (8 * 1024) << " KB" << std::endl;
}

void bench_roaring(int cardinality)
{
    Roaring r;
    for(auto i = 0; i <= 100000000; i++) {
        if(i % cardinality == 0) r.add(i);
    }

    r.setCopyOnWrite(true);
    r.runOptimize();


    auto counter = 0;
    auto start = std::chrono::steady_clock::now();
    for(Roaring::const_iterator i = r.begin() ; i != r.end() ; i++) ++counter;
    auto end = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Roaring Entries : " << counter
        << " Time: " << elapsed.count()
        << " Size: " << r.getSizeInBytes() / (1024)  << " KB" << std::endl;
}

int main(int argc, char const *argv[])
{

    for(auto i = 10; i <= 100000; i*=10) {
        std::cout << "Cardinality : 1 in " << i << " bits" << std::endl;
        bench_roaring(i);
        bench_ewah(i);
        std::cout << std::endl;
    }
    return 0;
}