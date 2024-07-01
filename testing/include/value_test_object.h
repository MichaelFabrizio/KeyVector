#pragma once

// Will be changed to Value_Test_Object
// Plan is to implement greater support for destructor testing
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
