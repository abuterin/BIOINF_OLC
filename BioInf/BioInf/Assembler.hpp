#pragma once
#include<vector>
#include<unordered_set>

#include "DovetailOverlap.hpp"

#define IS_EQUAL(x,y,error) ((y <= x + error) && (x <= y + error))
#define ABS(x) ((x < 0) ? x * (-1) : x)

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
	*/
	void filterShortOverlaps(std::vector<DovetailOverlap*>&, double);

	/**
		Implemented by Ante
	*/
	void filterErroneousOverlaps(std::vector<DovetailOverlap*>&, double);

	/**
		Implemented by Ante
		(Rahle, 2014)
	*/
	bool isTransitive(DovetailOverlap*, DovetailOverlap*, DovetailOverlap*);

	/**
		Implemented by Ante
		(Rahle, 2014)
	*/
	void filterTransitiveOverlaps(std::vector<DovetailOverlap*>&);

	~Assembler() { assembler = nullptr; }
};
