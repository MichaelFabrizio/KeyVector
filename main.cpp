#include <cstdint>   // uintptr_t
#include <stdlib.h>  // aligned_alloc
#include <iostream>
#include <memory>
#include <chrono>
#include <cassert>

#include <Logic_Tests.h>
#include <randomized_set.h>
#include <sequencer.h>

int main()
{
	// Always true according to C and C++ standards
	// Confirmation purposes. If this isn't true
	assert(sizeof(unsigned char) == 1);
	assert(alignof(unsigned char) == 1);

	Pool<524288, 4096> pool;
	auto& TestKeyVector = pool.AddKeyVec<Castle, unsigned char, 256>();

	Randomized_Set randomSet = Randomized_Set<1, 256>(); // 1 to 255 is the max allowable range of keys
	randomSet.SetRange(50, 75);													 //	Restrict the range of generated keys
	randomSet.ShuffleRandom(200);												 // Shuffle should be appropriately sized for the range size
	//randomSet.Debug();
	auto& set = randomSet.GetKeys();
	
	Sequencer sequencer(TestKeyVector);
	sequencer.Store_Sequence(set, Operation::Add);
	sequencer.Process();
	bool test_result = sequencer.Test_Values();

	std::cout << "Sequencer Test Result: " << test_result << '\n';
	return 0;
}
