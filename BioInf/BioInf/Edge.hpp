#pragma once
#include "CommonHeaders.hpp"

using namespace std;

/**
Class created by Mirela
*/
class Edge {
public:
	int edgeId;
	MHAPOverlap* overlap;
	unsigned int sourceNode;
	unsigned int pairId;
	Edge* _pair;
	bool _inWalk;

	//unsigned int destinationNode;
	Edge(int _edgeId, MHAPOverlap* _overlap, unsigned int _sourceNode) : _inWalk{ false }, _pair{ nullptr } {
		cout << "Constructor called." << endl;
		edgeId = _edgeId;
		overlap = _overlap;
		sourceNode = _sourceNode;
	}


	unsigned int opposite(unsigned int vertexID);

	unsigned int getDestinationNode();

	Vertex* getDst() {  }//dovrsiti:::::::::::::::::::::::::::::::::::.

	MHAPOverlap* getOverlap() {
		return overlap;
	}

	bool isInWalk() { return _inWalk; }

	void setInWalk(bool inWalk) { _inWalk = inWalk; }

	Edge* pair() { return _pair; }
};
