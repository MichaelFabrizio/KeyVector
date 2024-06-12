#pragma once
#include <array>
#include <iostream> // For debugging purposes
#include <new>
#include <vector>

class BaseVec {
public:
  virtual ~BaseVec() {}
};

template <typename T, typename I, std::size_t N>
class KeyVector : public BaseVec {
  typedef std::size_t Key;

  inline void Checked_Lesser_Place(const Key key) { 
    auto swapkey = _indices[key];
    // Testing purposes - should never throw:
    if (swapkey < key) { throw "LessThanPlace invalid key result\n"; }

    if (swapkey == key) { return; } // Key was found
    
    // Implicit: Key not found
    if (swapkey > (_length + 1)) {
      _indices[key] = key;
      Unchecked_Greater_Place(key, swapkey);
      return;
    }

    // Implicit: swapkey == _length + 1
    _indices[key] = key;
    Unchecked_Equal_Place(key, swapkey);
  }

  inline void Checked_Equal_Place(const Key key) {
    if (_indices[key] != 0) { return; } // Key was found

    // Implicit: Key not found
    _indices[key] = key;
    
    // Place new object at _data[key]
    T* _data_head = reinterpret_cast<T*>(_data);
    ::new (_data_head + key) T();

    // Update internal length
    _length++;
  }

  inline void Checked_Greater_Place(const Key key) {
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

  inline void Unchecked_Equal_Place(const Key key, const Key swapkey) {
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

  inline void Unchecked_Greater_Place(const Key key, const Key swapkey) {
    
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

  inline void Remove_Lesser_Key(const Key key) {
    if (key != _indices[key]) { return; } // Key not found
    // Implicit: Key == _indices[Key]

    T* _data_head = reinterpret_cast<T*>(_data);

    auto end_key = _indices[_length];
    _indices[key] = end_key;

    // Call destructor on key to be removed
    (_data_head + key)->~T();

    // Move element from end to fill the gap
    _data_head[key] = _data_head[_length];

    if (end_key != _length) {
      _indices[_length] = 0;
      _indices[end_key] = key;
      _length--;
      return;
    }

    // Implicit: end_key == _length
    _indices[end_key] = 0;
    _length--;
  }

  inline void Remove_Equal_Key(const Key key) {
    if (key != _indices[key]) { return; } // Key not found

    T* _data_head = reinterpret_cast<T*>(_data);
    (_data_head + key)->~T();

    _indices[key] = 0;
    _length--;
  }

  inline void Remove_Greater_Key(const Key key) {
    auto key_location = _indices[key];
    if (key_location == 0) { return; } // Key not found
    
    T* _data_head = reinterpret_cast<T*>(_data);

    auto end_key = _indices[_length];
    _indices[key_location] = end_key;
    _indices[end_key] = key_location;
    _indices[key] = 0;
    
    (_data_head + key_location)->~T();

    _data_head[key_location] = _data_head[_length];

    if (end_key != _length) {
      _indices[_length] = 0;
      _length--;
      return;
    }

    _length--;
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
      Checked_Lesser_Place(key);
      return;
    }
    // Implicit bounds:
    // length < key < N

    if (key == (_length + 1)) {
      Checked_Equal_Place(key);
      return;
    }
    // Implicit bounds:
    // (length + 1) < key < N
   
    Checked_Greater_Place(key);
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
  
  void AddRange(const Key lower_bound, const Key upper_bound) {
    if (lower_bound >= upper_bound) { return; }
    if (upper_bound > _capacity)   { return; }

    for (Key i = lower_bound; i < upper_bound; i++) {
      Add(i);
    }
  }

  void BuildFromVector(std::vector<Key>& initial_vector) {
    if (initial_vector.size() > _capacity) { std::cout << "[BuildFromVector] Invalid initial_vector magnitude\n"; return; }
    for (auto key : initial_vector) {
      Add(key);
    }
  }

  T& Find(const Key key) {
    // Invalid key bounds
    if (key >= N) {
      return *std::launder(reinterpret_cast<T*>(_data));
    }  
    // Implicit bounds: 0 < key < N

    if (key <= _length) {
      if (_indices[key] == key) {
        return *std::launder(reinterpret_cast<T*>(_data + key * sizeof(T)));
      }
      return *std::launder(reinterpret_cast<T*>(_data));
    }

    // Implicit bounds: _length < key < N
    auto key_location = _indices[key];
    return *std::launder(reinterpret_cast<T*>(_data + key_location * sizeof(T)));
  }
  
  Key FindIndex(const Key key) {
    if (key >= N) { return 0; }

    if (key <= _length) {
      if (key != _indices[key]) { return 0;   }
      else                      { return key; }
    }

    // Implicit: Key > _length
    return _indices[key];
  }

  void Remove(const Key key) {
    if (key == 0 || key >= N) { return; } // Do not remove zero element

    // Implicit bounds: 0 < key < N
    if (key < _length) {
      Remove_Lesser_Key(key);
      return;
    }

    if (key == _length) {
      Remove_Equal_Key(key);
      return;
    }

    // Implicit bounds: _length < key < N
    Remove_Greater_Key(key);
  }

  void Clear() {
    T* _data_head = reinterpret_cast<T*>(_data);

    // Don't clear the zero element!!!
    for (int i = 1; i < (_length + 1); i++) {

      // Call each destructor for T
      (_data_head + i)->~T();

      // Clear extra indexed data
      Key stored_key = _indices[i];
      if (stored_key != i) {
        _indices[stored_key] = 0;
      }

      // Must reset all indices
      _indices[i] = 0;
    }

    _length = 0;
  }

  std::size_t Length() const { return _length; }
  std::size_t Capacity() const { return _capacity; }

  const std::array<I, N>& GetIndexArray() const { return _indices; }

  void Debug() {
    if (_length > 1000) {
      std::cout << ">1000 elements, skipping print\n";
      return;
    }

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
