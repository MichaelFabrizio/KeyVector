#include <Pool.h>
#include <vector>

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
		if (index != castles->FindIndex(test_key)) { return false; }

		// VALUE TESTING CRITERIA:
		auto& castle = castles->Find(test_key);
		if (castle.damage_level != 2525)	{ return false; }
		if (castle.armor_level != 3636)		{ return false; }
		
		return true;
	}
	
	// Checks that the test_key landed on the end location ( 1st possibility)
	bool Confirm_Key_At_End_Is_Valid(const Key test_key) {
		return Confirm_Key_At_Index_Is_Valid(test_key, castles->Length());
	}

	// Checks if test_key is well-bounded, 
	// And that it exists within the initial_keys vector
	bool Is_Valid_Initial_Vector(std::vector<Key>& initial_keys, Key test_key) {
		if (test_key >= castles->Capacity())	{ return false; }

		bool test_key_found = false;

		for (auto key : initial_keys) {
			if (key == test_key)	{ test_key_found = true; }
		}

		return test_key_found;
	}

	// Keeps trying to add new elements until _indices[_length + 1] == 0
	bool Try_Add_Elements_Until_Zeroed_Next_Index(const Key test_key) {
			while (true) {

				auto Next = castles->Length() + 1;

				if (Next == test_key)							{ return false; }
				if (Next == castles->Capacity())	{ return false; }

				auto Next_Index = castles->FindIndex(Next);
				if (Next_Index == 0) { return true; }

				else { castles->AddAny(); } // Continue to next loop

			}
	}
	
	// Keeps trying to add new elements until _indices[_length + 1] == Some_Value
	bool Try_Add_Elements_Until_Active_Next_Index(const Key test_key) {
			while (true) {

				auto Next = castles->Length() + 1;

				if (Next == test_key)							{ return false; }
				if (Next == castles->Capacity())	{ return false; }

				auto Next_Index = castles->FindIndex(Next);
				if (Next_Index != 0) { return true; }

				else { castles->AddAny(); } // Continue to next loop

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
	Logic_Tests() {
		castles = &pool.AddKeyVec<Castle, unsigned char, 256>();
	}

	void Test_Add_Any() {
		// TEST CASE 1: AddAny() with INDEXED END ELEMENT. 
		// In this test: _indices[7] = _indices[_Length + 1] = 1 (it is indexed)

		std::vector<std::size_t> test_1_keys { 7, 2, 3, 4, 5, 6 };
		castles->BuildFromVector(test_1_keys);
		
		auto added_key = castles->AddAny();

		if (castles->FindIndex(7) != 7) { _status = false; }
		else if (added_key != 1)				{ _status = false; }

		// TEST CASE 2: AddAny() with ZERO END ELEMENT
		castles->Clear();
		for (int i = 0; i < 10; i++) {
			castles->AddAny();
		}
		
		for (int i = 0; i < 10; i++) {
			if (castles->FindIndex(i) != i) { _status = false; }
		}
		
		castles->Clear();
	}
	
	void Test_Add_Any_2() {
		std::vector<std::size_t> test_1_keys { 7, 2, 3, 45, 5, 6, 15, 25 };
		castles->BuildFromVector(test_1_keys);

		Key added_key = 0;
		while (true) {
			auto location_45 = castles->FindIndex(45);

			if (location_45 != 4) {
				if (added_key != 4) { _status = false; castles->Debug(); }
				castles->Clear();
				return;
			}
			else {
				added_key = castles->AddAny();
			}
		}

	}

	void Test_Checked_Equal_To_Placement() { 
		castles->AddRange(1, 128);

		for (int i = 1; i < 128; i++) {
			std::size_t index = castles->FindIndex(i);
			

			// Test succeeded for this i value 
			if (index == i) { continue; }
			// Test has failed
			else { _status = false; }
		}
		castles->Clear();
	}

	void Test_Checked_Greater_Than_Placement(std::vector<Key>& initial_keys, Key test_key, bool require_indexed_end_element) {
		// INITIAL KEYVECTOR SETUP
		Key Next_Element = castles->Length() + 1;
		if (test_key <= Next_Element) { std::cout << "Invalid test key\n"; return; }

		bool valid = Is_Valid_Initial_Vector(initial_keys, test_key);
		if (!valid) { std::cout << "Invalid test key\n"; return; }

		castles->BuildFromVector(initial_keys);
		
		valid = Try_Generate_Length_Plus_One_State(*castles, test_key, require_indexed_end_element);
		if (!valid) { std::cout << "Unsuitable initial_keys vector\n"; _status = false; return; } // Failed to get good KeyVector state

		// WRITE DATA FOR TEST KEY
		auto& castle = castles->Find(test_key);
		castle.damage_level = 2525; // Need a better way to test custom values...
		castle.armor_level = 3636;

		// GET RELEVANT KEYS THAT ARE POSSIBLY AFFECTED
		Key overwrite_key = castles->FindIndex(test_key);
		Key indexed_end_element = castles->FindIndex(Next_Element);
		
		// BUMP TEST KEY TO A NEW POSITION
		castles->Add(overwrite_key);

		bool test_status;
		if (!require_indexed_end_element) {
			test_status = Confirm_Key_At_End_Is_Valid(test_key);
		}
		else {
			test_status = Confirm_Key_At_Index_Is_Valid(test_key, indexed_end_element);
		}

		if (!test_status) { _status = false; }

		castles->Clear();
	}

	bool Return_Test_Status() {
		return _status;
	}

private:
	Pool<524288, 4096> pool;
	KeyVector<Castle, unsigned char, 256>* castles;

	// Eventually change to a vector<string TestName, bool Result> (so we can store multiple failed tests)
	bool _status = true;

};
