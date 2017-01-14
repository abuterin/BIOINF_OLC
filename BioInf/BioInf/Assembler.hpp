#pragma once
#include<vector>
#include<unordered_set>
#include<map>

#include "Read.hpp"
#include "DovetailOverlap.hpp"

#define IS_EQUAL(x,y,error) ((y <= x + error) && (x <= y + error))
#define ABS(x) ((x < 0) ? x * (-1) : x)

using namespace std;

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
	void filterContained(vector<DovetailOverlap*>&, map<unsigned int, Read*>&);

	/**
		Implemented by Ante
	*/
	void filterShortOverlaps(vector<DovetailOverlap*>&, double);

	/**
		Implemented by Ante
	*/
	void filterErroneousOverlaps(vector<DovetailOverlap*>&, double);

	/**
		Implemented by Ante
		(Rahle, 2014)
	*/
	bool isTransitive(DovetailOverlap*, DovetailOverlap*, DovetailOverlap*);

	/**
		Implemented by Ante
		(Rahle, 2014)
	*/
	void filterTransitiveOverlaps(vector<DovetailOverlap*>&);

	~Assembler() { assembler = nullptr; }
};
