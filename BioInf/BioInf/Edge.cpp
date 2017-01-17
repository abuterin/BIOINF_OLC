#include "Edge.hpp"

unsigned int Edge::opposite(unsigned int vertexID) {
	if (overlap->aID() == vertexID) {
		return overlap->bID();
	}
	else if (overlap->bID() == vertexID) {
		return overlap->aID();
	}
}

int Edge::labelLength() {
	if (labelLength_ > -1) {
		return labelLength_;
	}

	std::string l;
	label(l);
	labelLength_ = l.length();

	//assert(labelLength_ == abs(overlap->a_hang()) || labelLength_ == abs(overlap->b_hang()));

	return labelLength_;
}

void Edge::label(string & dst) {
	if (source->getId() == overlap->aID()) {
		// from A to B
		int start, len;

		if (overlap->isInnie()) {

			if (overlap->isUsingSuffix(destiantion->getId())) {
				start = overlap->length(destiantion->getId());
				len = overlap->bHang();
			}
			else {
				start = 0;
				len = -1 * overlap->aHang();
			}

		}
		else {

			if (overlap->isUsingSuffix(destiantion->getId())) {
				start = 0;
				len = -1 * overlap->aHang();
			}
			else {
				start = overlap->length(destiantion->getId());
				len = overlap->bHang();
			}
		}

		dst = (overlap->isInnie() ? destiantion->getReverseComplement() : destiantion->getSequence()).substr(start, len);

	}
	else {
		// from B to A
		int start, len;

		if (overlap->isUsingSuffix(destiantion->getId())) {
			start = 0;
			len = overlap->aHang();
		}
		else {
			start = overlap->length(destiantion->getId());
			len = -1 * overlap->bHang();
		}

		dst = destiantion->getSequence().substr(start, len);
	}
}
