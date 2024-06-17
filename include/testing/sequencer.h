#pragma once
#include <vector>
#include <iostream>

// Will be moved and expanded
enum class Operation {
	Add,
	Remove,
};

class Sequencer {
	typedef std::size_t Key;
	typedef std::vector<std::vector<Key>> Sequences;

public:
	inline void Add_Sequence(std::vector<Key>& keys, Operation operation) {

		switch (operation) {
			case Operation::Add:
					_sequences.push_back(keys);
					_operation_sequence.push_back(Operation::Add);
					break;
			case Operation::Remove:
					_sequences.push_back(keys);
					_operation_sequence.push_back(Operation::Remove);
					break;
			}
	}

private:
	Sequences _sequences;
	std::vector<Operation> _operation_sequence;
};
