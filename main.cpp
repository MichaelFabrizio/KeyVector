#include <cstdint>   // uintptr_t
#include <stdlib.h>  // aligned_alloc
#include <iostream>
#include <memory>
#include <chrono>
#include <cassert>

#include <Logic_Tests.h>

int main()
{
	// Always true according to C and C++ standards
	// Confirmation purposes. If this isn't true
	assert(sizeof(unsigned char) == 1);
	assert(alignof(unsigned char) == 1);

	// Test class which allocates a memory Pool and runs index calculations
	Logic_Tests test_placement_logic;
	
	typedef std::size_t Key;

	Key primary_test_key = 45;
	std::vector<std::size_t> initial_keys { 7, 2, 3, 45, 5, 6, 15, 25 };
	
	test_placement_logic.Test_Clear();
	
	test_placement_logic.Test_Add_Any();
	test_placement_logic.Test_Add_Any_2();
	test_placement_logic.Test_Add_Any_3();

	test_placement_logic.Test_Checked_Equal_To_Placement();
	test_placement_logic.Test_Checked_Greater_Than_Placement(initial_keys, primary_test_key, false);
	
	test_placement_logic.Test_Remove_Greater_Key_Branch_1();
	test_placement_logic.Test_Remove_Greater_Key_Branch_2(initial_keys, primary_test_key);

	//UNFINISHED:
	//test_placement_logic.Test_Checked_Greater_Than_Placement(initial_keys, primary_test_key, true);
	
	std::cout << "Test status: " << test_placement_logic.Return_Test_Status()	<< '\n';

	return 0;
}
