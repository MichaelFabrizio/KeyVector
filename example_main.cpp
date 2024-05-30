#include <cstdint>   // uintptr_t
#include <stdlib.h>  // aligned_alloc
#include <iostream>
#include <memory>
#include <chrono>
#include <cassert>

#include <Pool.h>

struct Castle { 
	Castle() : damage_level(200), armor_level(50) {}
	unsigned int armor_level;
	unsigned int damage_level;
};

int main()
{
	// Always true according to C and C++ standards
	// Confirmation purposes. If this isn't true
	assert(sizeof(unsigned char) == 1);
	assert(alignof(unsigned char) == 1);

	typedef std::size_t Key;

	// AllocationSize: How much memory we need UPFRONT
	constexpr static std::size_t AllocationSize = 512 * 1024; // 512 KiB

	// CRUCIAL: ALLOCATIONS MUST HAPPEN IMMEDIATELY AT PROGRAM START
	Pool<AllocationSize, 4096> pool; // 4096 Byte Default Alignment (do not change)
	// Create our data container object
	// IN THIS EXAMPLE:
	//
	// Data type: Castle (declared above main - a simple struct)
	// # of requested keys = 256;
	// Max representable keys = 256
	auto& castles = pool.AddKeyVec<Castle, unsigned char, 256>();
	
	// Add an arbitrary castle key # to the structure
	castles.Add(82);

	// Get the castle object
	auto& _82 = castles.Find(82);

	// Set the damage level to an arbitrarily specific value
	_82.damage_level = 9001;

	// ADDING DATA TESTS
	for (int i = 1; i < 12; i++)
	{
		// 1) Builds a group of sequential castles using 'castles.Add(i)';
		castles.Add(i);
		
		// 2) Gets the castle object corresponding to key 'i'
		auto& castle = castles.Find(i);

		// 3) Sets the castle damage_level value equal to its own key 'i'
		castle.damage_level = i;
	}

	for (int i = 0; i < 12; i++)
	{
		auto& castle = castles.Find(i);
		std::cout << "Damage level: " << castle.damage_level << '\n';
	}
	_82 = castles.Find(82);
	std::cout << "Damage level: " << _82.damage_level << '\n';
	
	// This command prints the keys found within the structure
	// castles.Debug();
	
	// Useful case to consider:
	// (char[N] Type would need stronger guarantees)!
	// auto& Characters = pool.AddHSet<char[32], unsigned char, 256>();

	return 0;
}
