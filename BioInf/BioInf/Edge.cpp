#include "Edge.hpp"

Edge::Edge(int _edgeId, MHAPOverlap * _overlap, unsigned int _sourceNode) {
	cout << "Constructor called." << endl;
	edgeId = _edgeId;
	overlap = _overlap;
	sourceNode = _sourceNode;
}

unsigned int Edge::opposite(unsigned int vertexID) {
	if (overlap->aID() == vertexID) {
		return overlap->bID();
	}
	else if (overlap->bID() == vertexID) {
		return overlap->aID();
	}
}