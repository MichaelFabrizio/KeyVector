#pragma once
#include <array>
#include <iostream> // For debugging purposes
#include <new>

class BaseVec {
public:
  virtual ~BaseVec() {}
};

template <typename T, typename I, std::size_t N>
class KeyVector : public BaseVec {
  typedef std::size_t Key;

  inline void CLP(const Key key) { 
    auto swapkey = _indices[key];
    // Testing purposes - should never throw:
    if (swapkey < key) { throw "LessThanPlace invalid key result\n"; }

    if (swapkey == key) { return; } // Key was found
    
    // Implicit: Key not found
    if (swapkey > (_length + 1)) {
      _indices[key] = key;
      UGP(key, swapkey);
      return;
    }

    // Implicit: swapkey == _length + 1
    _indices[key] = key;
    UEP(key, swapkey);
  }

  inline void CEP(const Key key) {
    if (_indices[key] != 0) { return; } // Key was found

    // Implicit: Key not found
    _indices[key] = key;
    
    // Place new object at _data[key]
    T* _data_head = reinterpret_cast<T*>(_data);
    ::new (_data_head + key) T();

    // Update internal length
    _length++;
  }

  // TODO : Needs testing
  inline void CGP(const Key key) {
    if (_indices[key] != 0) { return; } // Key was found

    // Get _data[0] pointer as a T* (its real type)
    T* _data_head = reinterpret_cast<T*>(_data);

    // The Key = _length + 1 was found in the data structure
    // It needs to be promoted to its home square
    Key index_pointer = _indices[_length + 1];
    if (index_pointer != 0) {
      // Update indices
      _indices[_length + 1] = _length + 1;
      _indices[index_pointer] = key;
      _indices[key] = index_pointer;

      // Move object at _data[index_pointer] to _data[_length + 1] (its home square)
      _data_head[_length + 1] = _data_head[index_pointer];

      // Place new object at _data[index_pointer]
      ::new (_data_head + index_pointer) T();
      
      // Update internal length
      _length++;
      return;
    }
    // Implicit: Key = _length + 1 not found within data structure
    // IE: _indices[_length + 1] == 0

    // Update indices
    _indices[_length + 1] = key;
    _indices[key] = _length + 1;

    // Place new object at _data[_length + 1]
    ::new (_data_head + _length + 1) T();
    
    // Update internal length
    _length++;
  }

  // TODO : Needs testing
  inline void UEP(const Key key, const Key swapkey) {
    _indices[swapkey] = swapkey;
    
    // Get _data[0] pointer as a T* (its real type)
    T* _data_head = reinterpret_cast<T*>(_data);

    // Move object at _data[key] into _data[swapkey]
    _data_head[swapkey] = _data_head[key];

    // Place new object at _data[key]
    ::new (_data_head + key) T();

    // Update internal length
    _length++;
  }

  // TODO : Needs testing
  inline void UGP(const Key key, const Key swapkey) {
    
    // Get _data[0] pointer as a T* (its real type)
    T* _data_head = reinterpret_cast<T*>(_data);

    Key index_pointer = _indices[_length + 1];
    if (index_pointer != 0) {
      _indices[_length + 1] = _length + 1;
      _indices[index_pointer] = swapkey;
      _indices[swapkey] = index_pointer;
      
      _data_head[_length + 1] = _data_head[index_pointer];
      _data_head[index_pointer] = _data_head[key];
      
      ::new (_data_head + key) T();

      _length++;
      return;
    }

    // Implicit: Key = _length + 1 not found within data structure
    // IE: _indices[_length + 1] == 0
    
    _indices[_length + 1] = swapkey;
    _indices[swapkey] = _length + 1;

    _data_head[_length + 1] = _data_head[key];

    ::new (_data_head + key) T();

    _length++;
  }

public:
  KeyVector() : _length(0), _capacity(N), _indices {0} {
    // Default initialize _data[0] element
    T* _data_head = reinterpret_cast<T*>(_data);
    ::new (_data_head) T();
  }

  ~KeyVector() {
    T* _data_head = reinterpret_cast<T*>(_data);

    for (int i = 0; i < (_length + 1); i++) {
      (_data_head + i)->~T();
    }
  }

  void Add(const Key key) {
    if (key == 0 || key >= N)       { return; } // Invalid key bounds (0 will require it's own methods)
    if ((_length + 1) >= _capacity) { return; } // KeyVector is full
    // Implicit bounds:
    // 1 <= key < N

    if (key <= _length) {
      CLP(key);
      return;
    }
    // Implicit bounds:
    // length < key < N

    if (key == (_length + 1)) {
      CEP(key);
      return;
    }
    // Implicit bounds:
    // (length + 1) < key < N
   
    // Only possibility left
    // TODO: Should be moved above CEP (much higher chance of being true)
    // Could be helpful for branch prediction
    CGP(key);
  }

  Key AddAny() {
    if ((_length + 1) >= _capacity) { return 0; } // KeyVector is full
    
    T* _data_head = reinterpret_cast<T*>(_data);

    Key index_pointer = _indices[_length + 1];
    _indices[_length + 1] = _length + 1;
    
    if (index_pointer != 0) {
      _indices[index_pointer] = index_pointer;

      _data_head[_length + 1] = _data_head[index_pointer];

      ::new (_data_head + index_pointer) T();

      _length++;
      return index_pointer;
    }

    ::new (_data_head + _length + 1) T();

    _length++;
    return _length;
  }

  T& Find(const Key key) {
    // Invalid key bounds
    if (key >= N) {
      std::cout << "Out of bounds key location:" << '\n';
      std::cout << "Returning _data[0] default" << '\n';
      return *std::launder(reinterpret_cast<T*>(_data));
    }  
    // Implicit bounds: 0 < key < N

    if (key <= _length) {
      if (_indices[key] == key) {
        return *std::launder(reinterpret_cast<T*>(_data + key * sizeof(T)));
      }
      return *std::launder(reinterpret_cast<T*>(_data));
    }

    auto key_location = _indices[key];
    std::cout << "Key: " << key << " Located at: " << (std::size_t)key_location << '\n';
    return *std::launder(reinterpret_cast<T*>(_data + key_location * sizeof(T)));
  }

  void Debug() {
    for (int i = 0; i < (_length + 1); i++) {
      std::cout << (std::size_t)_indices[i] << ", ";

      if (i % 10 == 0) {
      std::cout << '\n';
      }
    }
  }
private:
  std::size_t _length;
  std::size_t _capacity;
  std::array<I, N> _indices;
  alignas(T) std::byte _data[N * sizeof(T)];
};
