#include <cassert>
#include <logic_tests.h>
#include <Pool.h>

int main() {
	// Always true according to C and C++ standards
	// Confirmation purposes. If this isn't true
	assert(sizeof(unsigned char) == 1);
	assert(alignof(unsigned char) == 1);

	typedef std::size_t Key;

	// AllocationSize: How much memory we need UPFRONT
	constexpr static std::size_t AllocationSize = 512 * 1024; // 512 KiB

	// CRUCIAL: ALLOCATIONS MUST HAPPEN IMMEDIATELY AT PROGRAM START
	Pool<AllocationSize, 4096> pool; // 4096 Byte Default Alignment (do not change)
	auto& castles = pool.AddKeyVec<Castle, unsigned char, 256>();

	Logic_Tests tests(castles);

	tests.Test_Clear();
	tests.Test_Add_Any();
	tests.Test_Add_Any_2();
	tests.Test_Add_Any_3();

	tests.Test_Add_Equal_Key();
	
	tests.Test_Add_Lesser_Key_Branch_1();
	tests.Test_Add_Lesser_Key_Branch_2();
	tests.Test_Add_Lesser_Key_Branch_3();

	tests.Test_Add_Greater_Key_Branch_1();
	tests.Test_Add_Greater_Key_Branch_2();

	tests.Test_Remove_Greater_Key_Branch_1();

	std::vector<std::size_t> initial_keys { 17, 2, 3, 45, 5, 6, 15, 25 };
	Key test_key = 45;

	//This is the only one with generalizable keys
	tests.Test_Remove_Greater_Key_Branch_2(initial_keys, test_key);

	tests.Apply_Random_Key_Operation(1, 256, Operation::Add);
	tests.Test_Values();
	tests.Apply_Random_Key_Operation(1, 100, Operation::Remove);
	tests.Test_Values();

	std::cout << "Test status: " << tests.Return_Test_Status() << '\n';

	return 0;
}
