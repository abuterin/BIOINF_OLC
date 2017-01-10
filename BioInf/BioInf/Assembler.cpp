#include "Assembler.hpp"

Assembler* Assembler::assembler = nullptr; //initialization of private static member 'assembler'

DovetailOverlap* Assembler::calculateForcedHangs(unsigned int aLow, unsigned int aHigh, unsigned int aLen, unsigned int bLow, 
	unsigned int bHigh, unsigned int bLen, unsigned int Aid, unsigned int Bid)
{
	int aHang = aLow - bLow;				//a_hang
	unsigned int bAfter = bLen - bHigh;		//number of bases B has after overlap
	unsigned int aAfter = aLen - aHigh;		//number of bases A has after overlap
	int bHang = bAfter - aAfter;		//b_hang

	return new DovetailOverlap(aHang, bHang, Aid, Bid);
}

void Assembler::filterContained(std::vector<DovetailOverlap*>& overlaps) {
	std::unordered_set<unsigned int>* containedReads = new std::unordered_set<unsigned int>();
	size_t index;

	for (DovetailOverlap* overlap : overlaps) {
		if (overlap->aHang() <= 0 && overlap->bHang() >= 0) {
			containedReads->insert(overlap->Aid());
		}
		else if (overlap->aHang() >= 0 && overlap->bHang() <= 0) {
			containedReads->insert(overlap->Bid());
		}
	}

	index = 0;

	for (size_t i = 0; i < overlaps.size(); i++) {
		if (containedReads->find(overlaps[i]->Aid()) != containedReads->end()) { //read A found
			continue;
		}
		else if (containedReads->find(overlaps[i]->Bid()) != containedReads->end()) {//read B found
			continue;
		}

		overlaps[index] = overlaps[i];
		index++;
	}

	overlaps.resize(index);

}
