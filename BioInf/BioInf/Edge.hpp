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
	Edge(int _edgeId, DovetailOverlap* _overlap, unsigned int srcId, Graph* gph) : _inWalk{ false }, _pair{ nullptr }, 
		graph{ gph }, sourceId{ srcId }, edgeId{ _edgeId }, overlap{ _overlap }, labelLength_{ -1 } {
		//cout << "Constructor called." << endl;
		source = gph->getVertexById(srcId);
		destiantion = gph->getVertexById(overlap->aID() == srcId? overlap->bID(): overlap->aID());
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

	unsigned int opposite(unsigned int vertexID);

	int labelLength();

	void label(string& dst);

	void rkLabel(string& dst);
};
