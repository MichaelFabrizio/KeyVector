#pragma once

#include <random>

class Randomized_Set {

public:
	Randomized_Set(int a, int b) : gen(rd()), distrib(a, b) {}

	void GetRandom() {
    for (int n = 0; n != 10; ++n)
        std::cout << distrib(gen) << ' ';
    std::cout << '\n';
	}

private:
    std::random_device rd;  // a seed source for the random number engine
    std::mt19937 gen; // mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> distrib;
};
