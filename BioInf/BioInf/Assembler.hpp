#pragma once
#include<vector>
#include<unordered_set>

#include "DovetailOverlap.hpp"

/**
 Class created by  Ante
*/
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
	/**
		Implemented by Ante
	*/
	DovetailOverlap*  calculateForcedHangs(MHAPOverlap&);

	/**
		Implemented by Ante
	*/
	void filterContained(std::vector<DovetailOverlap*>&);
	/**
		Implemented by Ante
		(Rahle, 2014)
	*/
	void filterTransitiveOverlaps(std::vector<DovetailOverlap*>&);

	void filterShortOverlaps(std::vector<DovetailOverlap*>&, double);

	/**
		Implemented by Ante
	*/
	void filterErroneousOverlaps(std::vector<DovetailOverlap*>&, double);

	~Assembler() { assembler = nullptr; }
};
