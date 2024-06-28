#include <Pool.h>
#include <vector>
#include <iterator> // For std::forward_iterator_tag
#include <cstddef>  // For std::ptrdiff_t

struct Castle { 
	Castle() : damage_level(200), armor_level(50) {}
	unsigned int armor_level;
	unsigned int damage_level;

	bool operator==(const Castle& rhs) {
		bool maybe_equal = true;

		if (this->armor_level != rhs.armor_level)		{ maybe_equal = false; }
		if (this->damage_level != rhs.damage_level) { maybe_equal = false; }

		return maybe_equal;
	}
};

// Testing class: Allocates memory, writes data in a certain way, reads values
class Logic_Tests {
	typedef std::size_t Key;
	typedef KeyVector<Castle, unsigned char, 256> KeyVec;

	// Checks that the test_key landed at a specified inner location ( 2nd possibility)
	bool Confirm_Key_At_Index_Is_Valid(const Key test_key, const Key index) {
		// INDEX TESTING CRITERIA:
		if (index != testkeyvector.FindIndex(test_key)) { return false; }

		// VALUE TESTING CRITERIA:
		auto& castle = testkeyvector.Find(test_key);
		if (castle.damage_level != 2525)	{ return false; }
		if (castle.armor_level != 3636)		{ return false; }
		
		return true;
	}
	
	// Checks that the test_key landed on the end location ( 1st possibility)
	bool Confirm_Key_At_End_Is_Valid(const Key test_key) {
		return Confirm_Key_At_Index_Is_Valid(test_key, testkeyvector.Length());
	}

	// Checks if test_key is well-bounded, 
	// And that it exists within the initial_keys vector
	bool Is_Valid_Initial_Vector(std::vector<Key>& initial_keys, Key test_key) {
		if (test_key >= testkeyvector.Capacity())	{ return false; }

		bool test_key_found = false;

		for (auto key : initial_keys) {
			if (key == test_key)	{ test_key_found = true; }
		}

		return test_key_found;
	}

	// Keeps trying to add new elements until _indices[_length + 1] == 0
	bool Try_Add_Elements_Until_Zeroed_Next_Index(const Key test_key) {
			while (true) {

				auto Next = testkeyvector.Length() + 1;

				if (Next == test_key)							{ return false; }
				if (Next == testkeyvector.Capacity())	{ return false; }

				auto Next_Index = testkeyvector.FindIndex(Next);
				if (Next_Index == 0) { return true; }

				else { testkeyvector.AddAny(); } // Continue to next loop

			}
	}
	
	// Keeps trying to add new elements until _indices[_length + 1] == Some_Value
	bool Try_Add_Elements_Until_Active_Next_Index(const Key test_key) {
			while (true) {

				auto Next = testkeyvector.Length() + 1;

				if (Next == test_key)							{ return false; }
				if (Next == testkeyvector.Capacity())	{ return false; }

				auto Next_Index = testkeyvector.FindIndex(Next);
				if (Next_Index != 0) { return true; }

				else { testkeyvector.AddAny(); } // Continue to next loop

			}
	}

	// Tries to obtain a valid "indexed end-point" state
	// Returns false when there is no viable state
	bool Try_Generate_Length_Plus_One_State(KeyVec& castles, Key test_key, bool require_indexed_end_element) {
		if (require_indexed_end_element) {
			std::cout << "Activate promotion attempt\n";
			return Try_Add_Elements_Until_Active_Next_Index(test_key);
		}
		else {
			return Try_Add_Elements_Until_Zeroed_Next_Index(test_key);
		}
	}

public:
	Logic_Tests(KeyVec& TestKeyVector) : testkeyvector(TestKeyVector) {
	}

	void Test_Add_Any() {
		// TEST CASE 1: AddAny() with INDEXED END ELEMENT. 
		// In this test: _indices[7] = _indices[_Length + 1] = 1 (it is indexed)

		std::vector<std::size_t> test_keys { 7, 2, 3, 4, 5, 6 };
		testkeyvector.BuildFromVector(test_keys);
		
		auto added_key = testkeyvector.AddAny();

		if (testkeyvector.FindIndex(7) != 7) { _status = false; }
		else if (added_key != 1)				{ _status = false; }
		
		testkeyvector.Clear();
	}
	
	void Test_Add_Any_2() {
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

	void Test_Add_Any_3() {
		for (int i = 0; i < 10; i++) {
			testkeyvector.AddAny();
		}
		
		for (int i = 0; i < 10; i++) {
			if (testkeyvector.FindIndex(i) != i) { _status = false; }
		}
		
		testkeyvector.Clear();
	}
	
	void Test_Add_Greater_Key_Branch_1() {
		testkeyvector.AddRange(1, 10);
		testkeyvector.Add(13);

		auto& indices = testkeyvector.GetIndexArray();

		if (indices[10] != 13) { _status = false; }
		if (indices[13] != 10) { _status = false; }

		testkeyvector.Clear();
	}
	
	void Test_Add_Greater_Key_Branch_2() {
		testkeyvector.Add(13);
		testkeyvector.AddRange(2, 13);
		testkeyvector.Add(25);

		auto& indices = testkeyvector.GetIndexArray();

		if (indices[13] != 13)	{ _status = false; }
		if (indices[25] != 1)		{ _status = false; }
		if (indices[1] != 25)		{ _status = false; }

		testkeyvector.Clear();
	}

	void Test_Add_Equal_Key() { 
		testkeyvector.AddRange(1, 128);

		for (int i = 1; i < 128; i++) {
			std::size_t index = testkeyvector.FindIndex(i);
			

			// Test succeeded for this i value 
			if (index == i) { continue; }
			// Test has failed
			else { _status = false; }
		}
		testkeyvector.Clear();
	}

	void Test_Add_Lesser_Key_Branch_1() {
		auto& indices = testkeyvector.GetIndexArray();

		testkeyvector.AddRange(1, 6);
		testkeyvector.Add(25);
		testkeyvector.AddRange(7, 25);
		testkeyvector.Add(6);

		if (indices[6] != 6)		{ _status = false; }
		if (indices[25] != 25)	{ _status = false; }
		
		testkeyvector.Clear();
	}
	
	void Test_Add_Lesser_Key_Branch_2() {
		auto& indices = testkeyvector.GetIndexArray();

		testkeyvector.AddRange(1, 6);
		testkeyvector.Add(25);
		testkeyvector.Add(6);
		
		if (indices[6] != 6)	{ _status = false; }
		if (indices[7] != 25) { _status = false; }
		if (indices[25] != 7) { _status = false; }
		
		testkeyvector.Clear();
	}
	
	void Test_Add_Lesser_Key_Branch_3() {
		auto& indices = testkeyvector.GetIndexArray();

		testkeyvector.AddRange(1, 6);
		testkeyvector.Add(25);
		testkeyvector.Add(17);
		testkeyvector.AddRange(8, 17);
		testkeyvector.Add(6);
		
		if (indices[6] != 6)	{ _status = false; }
		if (indices[17] != 17) { _status = false; }
		if (indices[25] != 7) { _status = false; }
		if (indices[7] != 25) { _status = false; }
		
		testkeyvector.Clear();
	}

	void Test_Clear() {
		// Create an arbitrary KeyVector
		std::vector<std::size_t> test_keys { 17, 2, 3, 45, 5, 6, 15, 25 };
		testkeyvector.BuildFromVector(test_keys);

		testkeyvector.Clear();
		
		// Use GetIndexArray() method to get underlying _indices array
		auto& indices = testkeyvector.GetIndexArray(); // Type: std::array<unsigned char, 256>&
		
		// Every value in _indices must equal zero now.
		for (auto index : indices) {
			if (index != 0) {
				_status = false;
			}
		}
	}

	// Test case: Remove(25). Where _indices[_length] == 25
	void Test_Remove_Greater_Key_Branch_1() {
		std::vector<std::size_t> test_keys { 17, 2, 3, 45, 5, 6, 15, 25 };
		testkeyvector.BuildFromVector(test_keys);
		auto& indices = testkeyvector.GetIndexArray(); // Type: std::array<unsigned char, 256>&
		
		auto index = testkeyvector.FindIndex(25);
		auto& castle = testkeyvector.Find(25);
		castle.damage_level = 25;
		castle.armor_level = 52;

		testkeyvector.Remove(25);

		if (indices[index] != 0) {
			std::cout << "Endpoint not cleared, index: " << (std::size_t)indices[index] << '\n';
			_status = false;
		}
		if (indices[25] != 0) {
			_status = false;
		}
		testkeyvector.Clear();
		
	}

	void Test_Remove_Greater_Key_Branch_2(std::vector<Key>& initial_keys, Key test_key) {
		bool valid = Is_Valid_Initial_Vector(initial_keys, test_key);
		if (!valid) { std::cout << "Invalid test key\n"; return; }
		
		testkeyvector.BuildFromVector(initial_keys);
		auto& indices = testkeyvector.GetIndexArray(); // Type: std::array<unsigned char, 256>&
		auto length = testkeyvector.Length();

		Key test_key_index = testkeyvector.FindIndex(test_key);
		Key end_key = indices[length];
		
		// MUST SKIP THIS POSSIBLE BRANCH FOR THIS TEST
		if (test_key_index == length) { 
			std::cout << "[Test_Remove_Lesser_Key_Branch_2] Please fix initial_keys\n";
			_status = false;
			return;
		}

		// Implicit: _indices[test_key] < _length
		// Ready to begin test

		testkeyvector.Remove(test_key);

		if (indices[test_key_index] != end_key) { _status = false; }
		if (indices[length] != 0)								{ _status = false; }
		if (indices[test_key] != 0)							{ _status = false; }

		testkeyvector.Clear();
	}

	void Test_Remove_Lesser_Key_Branch_1() {
		auto& indices = testkeyvector.GetIndexArray();

		testkeyvector.AddRange(1, 51);
		
		testkeyvector.Remove(28);

		if (indices[28] != 50) { _status = false; }
		if (indices[50] != 28) { _status = false; }

		testkeyvector.Clear();
	}
	
	void Test_Remove_Lesser_Key_Branch_2() {
		auto& indices = testkeyvector.GetIndexArray();
		
		testkeyvector.AddRange(1, 45);
		testkeyvector.Add(50);

		testkeyvector.Remove(28);
		
		if (indices[28] != 50)	{ _status = false; }
		if (indices[45] != 0)		{ _status = false; }
		if (indices[50] != 28)	{ _status = false; }

		testkeyvector.Clear();
	}

	bool Return_Test_Status() {
		return _status;
	}

private:
	KeyVec testkeyvector;

	// Eventually change to a vector<string TestName, bool Result> (so we can store multiple failed tests)
	bool _status = true;

};
