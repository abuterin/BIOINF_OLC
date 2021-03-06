#include "Assembler.hpp"

Assembler* Assembler::assembler = nullptr; //initialization of private static member 'assembler'

DovetailOverlap* Assembler::calculateForcedHangs(MHAPOverlap& overlap)
{
	int aHang = overlap.aStart() - overlap.bStart();				//a_hang
	unsigned int bAfter = overlap.bLength() - overlap.bEnd();		//number of bases B has after overlap
	unsigned int aAfter = overlap.aLength() - overlap.aEnd();		//number of bases A has after overlap
	int bHang = bAfter - aAfter;		//b_hang

	return new DovetailOverlap(aHang, bHang, overlap);
}

void Assembler::filterContained(std::vector<DovetailOverlap*>& overlaps, map<unsigned int, Read*>& reads) {
	std::unordered_set<unsigned int>* containedReads = new std::unordered_set<unsigned int>();
	size_t index;

	for (DovetailOverlap* overlap : overlaps) {
		if (overlap->aHang() <= 0 && overlap->bHang() >= 0) {
			containedReads->insert(overlap->aID());
		}
		else if (overlap->aHang() >= 0 && overlap->bHang() <= 0) {
			containedReads->insert(overlap->bID());
		}
	}

	index = 0;
	vector<unsigned int> markedReads;

	for (size_t i = 0; i < overlaps.size(); i++) {
		if (containedReads->find(overlaps[i]->aID()) != containedReads->end()) { //read A found
			delete reads.find(overlaps[i]->aID())->second;
			reads.find(overlaps[i]->aID())->second = nullptr;
			markedReads.push_back(overlaps[i]->aID());
			//reads.erase(overlaps[i]->aID());			//remove sequence read also
			delete overlaps[i];
			continue;
		}
		else if (containedReads->find(overlaps[i]->bID()) != containedReads->end()) {//read B found
			delete reads.find(overlaps[i]->bID())->second;
			reads.find(overlaps[i]->bID())->second = nullptr;
			markedReads.push_back(overlaps[i]->bID());
			//reads.erase(overlaps[i]->bID());			//remove sequence read also
			delete overlaps[i];
			continue;	
		}

		overlaps[index] = overlaps[i];
		index++;
	}

	for (size_t i = 0; i < markedReads.size(); i++) {
		map<unsigned int, Read*>::iterator it = reads.find(markedReads[i]);
		if (it != reads.end()) {
			reads.erase(it);
		}	
	}

	overlaps.resize(index);

}

void Assembler::filterShortOverlaps(std::vector<DovetailOverlap*>& overlaps, double minCoverage) {
	size_t index = 0;
	
	for (size_t i = 0; i < overlaps.size(); i++) {
		if (overlaps[i]->coveredPercentageReadA() < minCoverage) {
			delete overlaps[i];
			continue;
		}
		else if (overlaps[i]->coveredPercentageReadB() < minCoverage) {
			delete overlaps[i];
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
		if (overlaps[i]->jaccardScore() >= maxError) {
			delete overlaps[i];
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
	if (f->aID() == g->bID()) { //A is readB in 'g' overlap
										//B is then readA in 'h' 
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
	else {	//A is readA in 'g' overlap
			//B is then readB in 'h' overlap
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

void Assembler::filterTransitiveOverlaps(std::vector<DovetailOverlap*>& overlaps) {
	
	std::unordered_set<DovetailOverlap*> transitiveOverlaps;

	for (DovetailOverlap* f : overlaps) {
		for (DovetailOverlap* g : overlaps) {
			if (g == f) { 
				continue;	//skip same overlap as f
			}

			if (f->aID() == g->aID()) {		//read A is the same in f and g
				for (DovetailOverlap* h : overlaps) {
					if (h == f || h == g) {
						continue; //skip f and g overlaps
					}

					if (f->bID() == h->aID() && g->bID() == h->bID()) {//read B is the same in f and h, and read C is the same in
						if (isTransitive(f, g, h)) {					//g and h
							transitiveOverlaps.insert(f);
						}
					}
					else if (f->bID() == h->bID() && g->bID() == h->aID()) {//read B is the same in f and h, and read C is the same in
						if (isTransitive(f, g, h)) {
							transitiveOverlaps.insert(f);
						}
					}
				}
			}
			else if (f->aID() == g->bID()) {		//read A in f is the same as read B in g
				for (DovetailOverlap* h : overlaps) {
					if (h == f || h == g) {
						continue; //skip f and g overlaps
					}

					if (f->bID() == h->aID() && g->aID() == h->bID()) {//read B is the same in f and h, and read C is the same in
						if (isTransitive(f, g, h)) {					//g and h
							transitiveOverlaps.insert(f);
						}
					}
					else if (f->bID() == h->bID() && g->aID() == h->aID()) {//read B is the same in f and h, and read C is the same in
						if (isTransitive(f, g, h)) {
							transitiveOverlaps.insert(f);
						}
					}
				}
			}
			
		}
	}

	size_t index = 0;
	for (size_t i = 0; i < overlaps.size(); i++) {
		if (transitiveOverlaps.find(overlaps[i]) != transitiveOverlaps.end()) {
			delete overlaps[i];
			continue;
		}

		overlaps[index] = overlaps[i];
		index++;
	}
	overlaps.resize(index);
}

