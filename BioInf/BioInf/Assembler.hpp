#pragma once
#include<vector>

class Assembler {
private:
	Assembler() {} //private constructor, for this is singleton

	static Assembler* assembler;

public:

	static Assembler* getAssembler() {
		if (assembler == nullptr) {
			assembler = new Assembler;
		}
		return assembler;
	}

	std::pair<int, int>*  calculateForcedHangs(unsigned int aLow, unsigned int aHigh, unsigned int aLen,
		unsigned int bLow, unsigned int bHigh, unsigned int bLen);

};
