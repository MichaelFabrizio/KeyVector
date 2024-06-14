#pragma once

#include <random>
#include <vector>

template <std::size_t MinKeyValue, std::size_t MaxKeyValue> // MaxKeyValue = The last valid element. For example, an array T[N] would have a MaxKeyValue of (N-1).
class Randomized_Set {

public:
    typedef std::size_t Key;

    Randomized_Set() : gen(rd()), distrib(MinKeyValue, MaxKeyValue) {
	static_assert(MinKeyValue < MaxKeyValue, "Invalid Randomized_Set bounds\n");
	for (int i = MinKeyValue; i < MaxKeyValue; i++) {
	    random_set.push_back(i);
	}
    }

    void ShuffleRandom(std::size_t count) {
	std::size_t reasonable_count = count;
	if (reasonable_count > 10000) {
	    reasonable_count = 10000;
	}

	for (int i = 0; i < reasonable_count; i++) {
	    std::size_t first_index = distrib(gen);
	    std::size_t second_index = distrib(gen);

	    std::size_t first_element = random_set[first_index];
	    std::size_t second_element = random_set[second_index];

	    random_set[first_index] = second_element;
	    random_set[second_index] = first_element;
	}
    }

    void Debug() {
	for (auto key : random_set) {
	    std::cout << "Key: " << key << '\n';
	}
    }

private:
    std::random_device rd;  // a seed source for the random number engine
    std::mt19937 gen; // mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> distrib;
    std::vector<std::size_t> random_set;
};
