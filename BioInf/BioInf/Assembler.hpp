#pragma once
#include<vector>
#include<unordered_set>

#include "DovetailOverlap.hpp"

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

	DovetailOverlap*  calculateForcedHangs(unsigned int aLow, unsigned int aHigh, unsigned int aLen,
		unsigned int bLow, unsigned int bHigh, unsigned int bLen, unsigned int Aid, unsigned int Bid);

	void filterContained(std::vector<DovetailOverlap*>&);

	~Assembler() { assembler = nullptr; }
};
