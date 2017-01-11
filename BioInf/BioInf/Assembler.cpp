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

void Assembler::filterTransitiveOverlaps(std::vector<DovetailOverlap*>& overlaps) {
}

void Assembler::filterShortOverlaps(std::vector<DovetailOverlap*>& overlaps, double minCoverage) {
	size_t index = 0;
	
	for (size_t i = 0; i < overlaps.size(); i++) {
		if (overlaps[i]->coveredPercentageReadA() < minCoverage) {
			continue;
		}
		else if (overlaps[i]->coveredPercentageReadB() < minCoverage) {
			continue;
		}

		overlaps[index] = overlaps[i];
		index++;
	}
	overlaps.resize(index);
}

void Assembler::filterErroneousOverlaps(std::vector<DovetailOverlap*>& overlaps, double maxError) {
	size_t index = 0;
	for (size_t i = 0; i < overlaps.size(); i++) {
		if (overlaps[i]->JaccardScore() >= maxError) {
			continue;
		}
		overlaps[index] = overlaps[i];
		index++;
	}
	overlaps.resize(index);
}

bool Assembler::isTransitive(DovetailOverlap * f, DovetailOverlap * g, DovetailOverlap * h) {
	unsigned int gHangA;
	unsigned int hHangC;
	unsigned int fHangA;
	unsigned int gHangC;
	unsigned int hHangB;
	unsigned int fHangB;
	double error = 0.1;
	double alpha = 0.1;
	//A is readA in 'f' overlap
	//B is readB in 'f' overlap
	if (f->Alength() == g->Blength()) { //A is readB in 'g' overlap, although this comparisson might not be precise
										//B is then readA in 'h' overlap, although this is suspicious as well
										//C is readA in 'g' overlap and readB in 'h' overlap
		if (g->suffixA() == h->suffixB()) {		//check node C
			return false;
		}
		if (g->suffixB() != f->suffixA() || h->suffixA() != f->suffixB()) {	//check nodes A and B
			return false;
		}
		gHangA = ABS(g->bHang());
		hHangC = ABS(h->bHang());
		fHangA = ABS(f->aHang());
		gHangC = ABS(g->aHang());
		hHangB = ABS(h->aHang());
		fHangB = ABS(f->bHang());
		if (!IS_EQUAL(gHangA + hHangC, fHangA, error * f->overlapLength() + alpha)) {
			return false;
		}

		return IS_EQUAL(gHangC + hHangB, fHangB, error * f->overlapLength() + alpha);
		
	}
	else {	//A is readA in 'g' overlap, although this comparisson might not be precise
			//B is then readB in 'h' overlap, although this is suspicious as well
			//C is readB in 'g' overlap and readA in 'h' overlap

		if (g->suffixB() == h->suffixA()) {		//check node C
			return false;
		}
		if (g->suffixA() != f->suffixA() || h->suffixB() != f->suffixB()) {	//check nodes A and B
			return false;
		}
		gHangA = ABS(g->aHang());
		hHangC = ABS(h->aHang());
		fHangA = ABS(f->aHang());
		gHangC = ABS(g->bHang());
		hHangB = ABS(h->bHang());
		fHangB = ABS(f->bHang());
		if (!IS_EQUAL(gHangA + hHangC, fHangA,error * f->overlapLength() + alpha)) {
			return false;
		}

		return IS_EQUAL(gHangC + hHangB, fHangB, error * f->overlapLength() + alpha);
	}

}
