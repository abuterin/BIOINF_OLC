#pragma once
#include "CommonHeaders.hpp"

using namespace std;

/**
Class created by Mirela
*/
class Edge {
public:
	int edgeId;
	DovetailOverlap* overlap;
	Vertex* source;
	Vertex* destiantion;
	unsigned int sourceId;
	unsigned int destinationId;
	unsigned int pairId;
	Edge* _pair;
	bool _inWalk;
	int labelLength_;
	Graph* graph;

	//unsigned int destinationNode;
	Edge(int _edgeId, DovetailOverlap* _overlap, unsigned int srcId, Graph* gph) : _inWalk{ false }, _pair{ nullptr } {
		//cout << "Constructor called." << endl;
		graph = gph;
		sourceId = srcId;
		edgeId = _edgeId;
		overlap = _overlap;
		labelLength_ = -1;
		source = gph->getVertex(srcId);
		destiantion = gph->getVertex(overlap->aID() == srcId? overlap->bID(): overlap->aID());
	}

	Vertex* getSrc() {
		return source;
	}
	Vertex* getDst() {
		return destiantion;
	}

	DovetailOverlap* getOverlap() {
		return overlap;
	}

	bool isInWalk() { return _inWalk; }

	void setInWalk(bool inWalk) { _inWalk = inWalk; }

	Edge* pair() { return _pair; }

	unsigned int opposite(unsigned int vertexID) {
		if (overlap->aID() == vertexID) {
			return overlap->bID();
		}
		else if (overlap->bID() == vertexID) {
			return overlap->aID();
		}
	}

	int labelLength() {

		if (labelLength_ > -1) {
			return labelLength_;
		}

		std::string l;
		label(l);
		labelLength_ = l.length();

		//assert(labelLength_ == abs(overlap->a_hang()) || labelLength_ == abs(overlap->b_hang()));

		return labelLength_;
	}

	void label(string& dst) {

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
};
