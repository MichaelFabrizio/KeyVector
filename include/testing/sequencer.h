#pragma once
#include <vector>
#include <iostream>

#include <value_test_object.h>

// Will be moved and expanded
enum class Operation {
	Add,
	Remove,
};

template <std::size_t MaxKeyValue>
class Sequencer {
	typedef std::size_t Key;
	typedef std::vector<std::vector<Key>> Sequences;
	typedef KeyVector<Castle, unsigned char, MaxKeyValue> KeyVec;

	void Process_Add_Sequence(std::vector<Key>& add_keys) {
		for (auto key : add_keys) {
			testkeyvector.Add(key);

			// Struct Castle will be replaced with a dedicated testing struct, works for now
			Castle& value = testkeyvector.Find(key);
			value.damage_level = key;
		}
	}

	void Process_Remove_Sequence(std::vector<Key>& remove_keys) {
		for (auto key : remove_keys) {
			testkeyvector.Remove(key);
		}
	}

	void Process_Switch_Cases(std::vector<Key>& keys, Operation current_operation) {
		switch (current_operation) {
			case Operation::Add:
				Process_Add_Sequence(keys);
				break;
			case Operation::Remove:
				Process_Remove_Sequence(keys);
				break;
			default:
				break;
		}
	}

public:
	Sequencer(KeyVec& TestKeyVector) : testkeyvector(TestKeyVector) {
		static_assert(MaxKeyValue < 10000, "MaxKeyValue restricted to <10000\n");
		auto& zero_value = testkeyvector.Find(0);
		zero_value.damage_level = 0;
	}

	void Clear() {
		_sequences.clear();
		_operation_sequence.clear();
	}

	void Store_Sequence(std::vector<Key>& keys, Operation operation) {
		switch (operation) {
			case Operation::Add:
					_sequences.push_back(keys);
					_operation_sequence.push_back(Operation::Add);
					break;
			case Operation::Remove:
					_sequences.push_back(keys);
					_operation_sequence.push_back(Operation::Remove);
					break;
			default:
					_sequences.push_back(keys);
					_operation_sequence.push_back(Operation::Add); // If somehow an enum is not one of the above options, default to an Add operation
				break;
			}
	}

	void Process() {
		if (_sequences.empty()) { return; }

		int i = 0;
		for (auto& vec : _sequences) {
			if (vec.empty()) { i++; continue; }

			Process_Switch_Cases(vec, _operation_sequence[i]);
			i++;
		}
	}

private:
	Sequences _sequences;
	std::vector<Operation> _operation_sequence;
	
	KeyVec& testkeyvector;
};
