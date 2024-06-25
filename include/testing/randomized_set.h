#pragma once

#include <random>
#include <vector>

template <std::size_t MinKeyValue, std::size_t MaxKeyValue> // MaxKeyValue = One-Past-End element. For example, an array T[N] would have a MaxKeyValue of N.
class Randomized_Set {
    typedef std::size_t Key;

public:

    Randomized_Set() : gen(rd()), _lower_bound(MinKeyValue), _upper_bound(MaxKeyValue)  {
	static_assert(MinKeyValue < MaxKeyValue, "Invalid Randomized_Set bounds\n");
	static_assert(MaxKeyValue < 10000, "MaxKeyValue restricted to <10000\n");
	for (int i = MinKeyValue; i < MaxKeyValue; i++) {
	    random_set.push_back(i);
	}
    }

    void ShuffleRandom(std::size_t count) {
	std::size_t reasonable_count = count;
	if (reasonable_count > 10000) {
	    reasonable_count = 10000;
	}

	std::uniform_int_distribution<Key> distrib(0, random_set.size() - 1);
	
	for (int i = 0; i < reasonable_count; i++) {
	    std::size_t first_index = distrib(gen);
	    std::size_t second_index = distrib(gen);

	    std::size_t first_element = random_set[first_index];
	    std::size_t second_element = random_set[second_index];

	    random_set[first_index] = second_element;
	    random_set[second_index] = first_element;
	}
    }

    void SetRange(Key lower_bound, Key upper_bound) {
	if (lower_bound >= upper_bound) { return; } // No change cases
	if (lower_bound < MinKeyValue) { return; }
	if (upper_bound > MaxKeyValue) { return; }

	_lower_bound = lower_bound;
	_upper_bound = upper_bound;

	random_set.clear();
	for (int i = _lower_bound; i < _upper_bound; i++) {
	    random_set.push_back(i);
	}
	
    }

    void SetShuffleMultiplier(std::size_t multiplier) {
	if (multiplier > 100) { return; }

	shuffle_multiplier = multiplier;
    }

    std::vector<Key>& GetKeys() { return random_set; }

    std::vector<Key>& GetShuffledKeys() { 
	std::size_t shuffle_count = shuffle_multiplier * (_upper_bound - _lower_bound);
	ShuffleRandom(shuffle_count);
	return random_set;
    }

    void Debug() {
	for (auto key : random_set) {
	    std::cout << "Key: " << key << '\n';
	}
    }

private:
    std::random_device rd;  // a seed source for the random number engine
    std::mt19937 gen; // mersenne_twister_engine seeded with rd()

    Key _lower_bound;
    Key _upper_bound;

    std::size_t shuffle_multiplier = 4;

    std::vector<std::size_t> random_set;
};
