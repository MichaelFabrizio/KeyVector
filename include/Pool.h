#pragma once
#include <iostream>
#include <stdlib.h>   // aligned_alloc
#include <algorithm>  // max
#include <vector>

#include <keyvector.h>

constexpr static std::size_t PAGE_SIZE = 4096;

constexpr static std::size_t MAX_SUPPORTED_ALIGN = 4096;              // 4 KiB - Maximum allocation alignment.
                                                                      // Modifiable by the end user - given they take responsibility for their own hardware memory limits.

constexpr static std::size_t MAX_SUPPORTED_SIZE = 1024 * 1024 * 1024; // 1 GiB - Maximum allocation size.
                                                                      // Modifiable by the end user - given they take responsibility for their own hardware memory limits.

// A no-exception pool
// *Must* be initialized at program start before all other processes.
// Aborts itself if anything goes wrong.
// If successful, end-user can create a guaranteed reference, for example: 
// auto& keyvec = AddKeyVec<Type, unsigned char, 256>()
//
// **All KeyVector references stay valid for the lifetime of Pool.
//
// N is the total pool size (memory allocation) in bytes
template <std::size_t N, std::size_t align>
class Pool {
public:
  // Params:
  // KeyVecSize - The quantity of unique ID keys this set will hold
  template<typename Type, typename Index, std::size_t KeyVecSize>
  KeyVector<Type, Index, KeyVecSize>& AddKeyVec() {
    static_assert (alignof(Index) <= PAGE_SIZE, "[Pool::AddKeyVec()] Index alignment isn't valid");
    static_assert (alignof(Type) <= PAGE_SIZE,  "[Pool::AddKeyVec()] Type alignment isn't valid");
    static_assert (sizeof(Index) <= PAGE_SIZE,  "[Pool::AddKeyVec()] Index size isn't valid");
    static_assert (sizeof(Type) <= PAGE_SIZE,   "[Pool::AddKeyVec()] Type size isn't valid");

    // Testing purposes
    static_assert ((sizeof(Index) % alignof(Index)) == 0);
    static_assert ((sizeof(Type) % alignof(Type)) == 0);
    
    constexpr std::size_t KeyVecAlignment = alignof(KeyVector<Type, Index, KeyVecSize>);
    constexpr std::size_t KeyVecByteSize = sizeof(KeyVector<Type, Index, KeyVecSize>);


    static_assert(KeyVecAlignment == std::max({ alignof(Type), alignof(Index), sizeof(std::size_t) }),
                  "[Pool::AddKeyVec()] KeyVecAlignment should equal the max alignment of the KeyVector member variables");

    // This static_assert is maybe unnecessary
    static_assert(PAGE_SIZE % KeyVecAlignment == 0);
    
    // Not needed, testing purposes
    std::size_t Full_Pages_Used = KeyVecByteSize / PAGE_SIZE;

    // Crucial value
    std::size_t Page_Overflow_Remainder = KeyVecByteSize % PAGE_SIZE;
    std::cout << "Full_Pages_Used: " << Full_Pages_Used << '\n';
    std::cout << "Page Overflow Remainder: " << Page_Overflow_Remainder << '\n';

    // We need to know how far to move the pointer forward
    std::size_t TotalBytesNeeded;
    // KeyVec object perfectly aligns with the page boundary, lucky!
    if (Page_Overflow_Remainder == 0) {
      TotalBytesNeeded = KeyVecByteSize;
    }
    // KeyVec fills up part of an extra page. Needs 1 full extra page of space.
    else {
      TotalBytesNeeded = KeyVecByteSize - Page_Overflow_Remainder + PAGE_SIZE;
    }
    std::cout << "TotalBytesNeeded: " << TotalBytesNeeded << '\n';
    std::cout << std::endl;
    
    // This one is tricky. Needs lots of consideration.
    // Checks if there is enough space left
    if (TotalBytesNeeded > (N - _filled_bytes)) {
      std::cout << "Out of memory\n";
      this->~Pool();
      // This abort is fatal for a no-exception pool and needs to stay
      // For Exception Handling Pools it may be a different implementation
      abort();
    }
    // Enough space exists -> Place KeyVec finally
    else {
      // Need to convert tracking_ptr into appropriate type -> To use with placement new
      KeyVector<Type, Index, KeyVecSize>* ptr_head = reinterpret_cast<KeyVector<Type, Index, KeyVecSize>*>(_tracking_ptr);

      KeyVector<Type, Index, KeyVecSize>* constructedObject = ::new (ptr_head) KeyVector<Type, Index, KeyVecSize>;

      _ptr_list.push_back(reinterpret_cast<BaseVec*>(constructedObject));
     
      // We need to keep track of all allocated bytes (for future space calculations)
      _filled_bytes += TotalBytesNeeded;

      // Update _tracking_ptr offset within the buffer
      unsigned char* ptr_tail = reinterpret_cast<unsigned char*>(_tracking_ptr);
      _tracking_ptr = reinterpret_cast<void*>(ptr_tail + TotalBytesNeeded);

      // Testing purposes - Must hold true
      assert(((unsigned char*)_tracking_ptr - (unsigned char*)_lead_ptr) % PAGE_SIZE == 0);
      std::cout << "keyvec start address: " << constructedObject << '\n';
      std::cout << "keyvec padded one-past-end address: " << _tracking_ptr << '\n';
      std::cout << "keyvec not padded one-past-end address: " << (constructedObject + 1) << '\n';
      std::cout << "keyvec successfully placed\n";
    std::cout << "~~~~~~~~~~~~~~~~~ \n";
    std::cout << std::endl;
      return *constructedObject;
    }
  }

  // Creates a memory resource with "N" overall bytes, "align" alignment, and PAGE_SIZE=4096 default
  // With the following restrictions:
  // N = a * align = b * PAGE_SIZE; for any unsigned int a and b
  //
  Pool() {
    if (N == 0 || align == 0) {
      std::cout << "Invalid Pool Size or Align of 0\n";
      abort();
    }

    if (align % PAGE_SIZE != 0) {
      std::cout << "Alignment must be multiple of 4096\n";
      abort();
    }

    if (N % align != 0) {
      std::cout << "Size must be multiple of align\n";
      abort();
    }

    // Need to avoid hardcoding the error message for MAX_SUPPORTED_SIZE
    static_assert(N <= MAX_SUPPORTED_SIZE, 
      "[Pool()] Requested allocation size is too large. MAX_SUPPORTED_SIZE = 1 GiB.");
    
    // Need to avoid hardcoding the error message for MAX_SUPPORTED_ALIGN
    static_assert(align <= MAX_SUPPORTED_ALIGN, 
      "[Pool()] Requested allocation align is too large. MAX_SUPPORTED_ALIGN = 4 KiB.");

    _lead_ptr = std::aligned_alloc(align, N);

    if (_lead_ptr == nullptr) {
      // Instead of abort really the whole program should be exited gracefully (cleaning up other memory)
      // Anyway, it works for now. (This is the only memory allocation)
      std::cout << "Bad allocation\n";
      abort();
    }

    // Memory is confirmed as allocated
    std::cout << "~~~~~~~~~~~~~~~~~ \n";
    std::cout << std::endl;
    std::cout << "New Memory Pool" << '\n';
    std::cout << "Memory allocated: " << N / 1024 << " KiB\n";
    std::cout << "Lead ptr: " << _lead_ptr << "\n";
    std::cout << "~~~~~~~~~~~~~~~~~ \n";
    std::cout << std::endl;

    _tracking_ptr = _lead_ptr;
  }
  ~Pool() {
    std::cout << "Freeing memory\n";

    for (auto ptr : _ptr_list) {
      ptr->~BaseVec();
    }

    // Deallocate the entire memory block
    free(_lead_ptr); 
  }

private:
  std::vector<BaseVec*> _ptr_list;

  void* _lead_ptr = nullptr;
  void* _tracking_ptr = nullptr;
  std::size_t _filled_bytes = 0;
};
