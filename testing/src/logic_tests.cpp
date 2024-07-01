#include <logic_tests.h>

void Logic_Tests::Test_Add_Any() {
	// TEST CASE 1: AddAny() with INDEXED END ELEMENT. 
	// In this test: _indices[7] = _indices[_Length + 1] = 1 (it is indexed)

	std::vector<std::size_t> test_keys { 7, 2, 3, 4, 5, 6 };
	testkeyvector.BuildFromVector(test_keys);
	
	auto added_key = testkeyvector.AddAny();

	if (testkeyvector.FindIndex(7) != 7) { _status = false; }
	else if (added_key != 1)				{ _status = false; }
	
	testkeyvector.Clear();
}

void Logic_Tests::Test_Add_Any_2() {
	std::vector<std::size_t> test_keys { 7, 2, 3, 45, 5, 6, 15, 25 };
	testkeyvector.BuildFromVector(test_keys);

	Key added_key = 0;
	while (true) {
		auto location_45 = testkeyvector.FindIndex(45);

		if (location_45 != 4) {
			if (added_key != 4) { _status = false; }
			testkeyvector.Clear();
			return;
		}
		else {
			added_key = testkeyvector.AddAny();
		}
	}
}
