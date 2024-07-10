#pragma once

#include <iostream>
#include <chrono> // For steady_clock

// Work in progress benchmarking class
// Will eventually support exporting data

// Regarding C++20 concepts - I would really rather avoid SFINAE as much as possible
// We might be updating to C++20 requirements in the near future, will open a github issue

// Must ensure the template type equals KeyVector<Type, IndexType, Size>
// (No guarantees this class works with any arbitrary type)

template <typename KeyVector>
class Benchmarks {
public:
  Benchmarks(KeyVector& keyvec) : _keyvec(keyvec) {
    std::cout << "Doesn't do anything! LOL\n";
  }
private:
  KeyVector& _keyvec;
};
