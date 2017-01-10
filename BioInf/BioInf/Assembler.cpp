#include "Assembler.hpp"

Assembler* Assembler::assembler = nullptr; //initialization of private static member 'assembler'

DovetailOverlap* Assembler::calculateForcedHangs(MHAPOverlap& overlap)
{
	int aHang = overlap.Astart() - overlap.Bstart();				//a_hang
	unsigned int bAfter = overlap.Blength() - overlap.Bend();		//number of bases B has after overlap
	unsigned int aAfter = overlap.Alength() - overlap.Aend();		//number of bases A has after overlap
	int bHang = bAfter - aAfter;		//b_hang

	return new DovetailOverlap(aHang, bHang, overlap);
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
