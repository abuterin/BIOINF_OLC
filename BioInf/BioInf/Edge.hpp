#pragma once
//#include "CommonHeaders.hpp"

#include "Vertex.hpp"
#include "DovetailOverlap.hpp"

class Graph;

using namespace std;

/**
Class created by Mirela
*/
class Edge {
private:
	bool _marked = false; //marked for deletion
	bool _inWalk;

public:
	int edgeId;
	DovetailOverlap* overlap;
	Vertex* source;
	Vertex* destination;
	unsigned int sourceId;
	unsigned int destinationId;
	unsigned int pairId;
	Edge* _pair;

	int labelLength_;
	Graph* graph;

	//unsigned int destinationNode;
	Edge(int _edgeId, DovetailOverlap* _overlap, unsigned int srcId, Graph* gph);

	Vertex* getSrc() {
		return source;
	}
	Vertex* getDst() {
		return destination;
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

	void mark() {
		_marked = true;
	}

	bool isMarked() { return _marked; }

	Vertex* oppositeVertex(unsigned int id);
};
