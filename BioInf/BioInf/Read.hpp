#pragma once
#include "CommonHeaders.hpp"

using namespace std;

/**
	Class created by Ante
*/
//#pragma region kCoder
/*************************************************************************************** /
* Title: RNA Assembler source code
* Author : rvaser (robert.vaser@gmail.com)
* Date : Apr 13, 2015
* Availability : https://github.com/mariokostelac/ra
****************************************************************************/

/*std::vector<char> kCoder = {
	-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
	-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
	-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
	-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
	-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
	-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
	-1,  -1,  -1,  -1,  -1,  65,  66,  67,  68,  69,
	70,  71,  72,  73,  74,  75,  76,  77,  78,  79,
	80,  81,  82,  83,  84,  85,  86,  87,  88,  89,
	90,  -1,  -1,  -1,  -1,  -1,  -1,  65,  66,  67,
	68,  69,  70,  71,  72,  73,  74,  75,  76,  77,
	78,  79,  80,  81,  82,  83,  84,  85,  86,  87,
	88,  89,  90,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
	-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
	-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
	-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
	-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
	-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
	-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
	-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
	-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
	-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
	-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
	-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
	-1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
	-1,  -1,  -1,  -1,  -1
};

#pragma endregion*/

class Read {
private:
	unsigned int _ID;
	string _read;//name
	string sequence_;
	string _reverseComplement;
	double _coverage;

public:
	Read(string read, unsigned int ID/*, const std::string& sequence*/) : _read{ read }, _ID{ ID }, _coverage{ 1.0 } {
		/*sequence_.reserve(sequence.size());
		for (uint32_t i = 0; i < sequence.size(); ++i) {
			auto c = kCoder[sequence[i]];
			if (c != -1) {
				sequence_.push_back(c);
			}
		}*/
	}

	string reverseComplement();

	string read() { return _read; }
	unsigned int ID() { return _ID; }
	double coverage() { return _coverage; }

	/*const string& sequence() const {
		return sequence_;
	}*/

};