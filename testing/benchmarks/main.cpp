#include <cassert>

#include <Pool.h>
#include <benchmarks.h>
#include <value_test_object.h>

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

	Benchmarks benchmarks(castles);
	return 0;
}
