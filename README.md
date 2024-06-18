# KeyVector
-----------
A single header datastructure to bootstrap an application with versatile memory:

| ***Access Pattern***                       | *Key Vector*    |
|--------------------------------------------|-----------------|
| Add                                        | O(1)            |
| Remove                                     | O(1)            |
| Find                                       | O(1)*           |
| N Element Iteration                        | O(N)            |
| Contiguous Key-Values                      | **✓**           |
| Pointer Stability                          |   ✘             |

**Warning** - Memory behavior is not yet guaranteed for real applications. Test at your own risk.

## Single header #include
``` C++
#include <keyvector.h>
```

## Non-allocating by default
``` C++
auto& keyvector = KeyVector<Type, unsigned char, 256>
```
Warning: allocated on the stack!

## Stack allocating class Pool<Size, Alignment>
**Must** be declared at program start. It will abort on errors.

A single allocated block of memory. Holds as many keyvectors as can be stored (without overflow).

Linux only for now (uses aligned_alloc).
``` C++
#include <Pool.h>
Pool<Allocation_Size, Alignment_Requirement> pool;
```

## Adding KeyVector<Type, Index, Size> to a Pool<Size, Alignment>
User **must** call the AddKeyVec<T, I, Size>() function **directly after** a Pool<Size, Alignment> is built.

``` C++
auto& keyvec_1 = pool.AddKeyVec<Type, unsigned char, 256>();
auto& keyvec_2 = pool.AddKeyVec<Type, short, 50000>();
```
These will also abort on any failed step. 

If it passes this stage without aborting, all references are valid. 

**Warning**: Type is intended to be Default Constructible, and Trivially Copy Constructible. Untested on any pointer Type objects, or objects which handle other resources..

## Add Keys:
Use Add(key) to easily add new key-value pairs:

``` C++
Castle& castles = pool.AddKeyVec<Castle, unsigned char, 256>();

// Add an arbitrary castle key # to the structure
castles.Add(82);

// Note: The Castle type is simply
// struct Castle { int Wall_Level = 0; }
```

In this case, the KeyVec can hold up to 256 unique keys (0-255) due to the Index type being unsigned char.
``` C++
// Note: The Castle type is simply
struct Castle { int Wall_Level = 0; }
```

## Find Keys
Access Values with Find(Key):
``` C++
// Get your value by reference
Castle& castle_82 = castles.Find(82);

// Read or write to it
castle_82.Wall_Level = 9001;
```

## Final considerations
Uses a special hashing algorithm designed by myself. Looking for research contributors.
Please report any runtime errors or memory errors, thank you.

![Partial_Timelapse](https://github.com/MichaelFabrizio/KeyVector/assets/83029804/7b0b9fe6-d774-47a7-95a8-51bf7d462b0a)

Author Note [6/18/24]: The KeyVector datastructure was researched fully independently by Mike Fabrizio. As of 6/18/24, all work in these libraries is attributable to the Author.

