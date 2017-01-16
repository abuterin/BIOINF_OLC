#include "Edge.hpp"

unsigned int Edge::opposite(unsigned int vertexID) {
	if (overlap->aID() == vertexID) {
		return overlap->bID();
	}
	else if (overlap->bID() == vertexID) {
		return overlap->aID();
	}
}
unsigned int Edge::getDestinationNode() {
	unsigned int dstNode;
	if (overlap->aID() == sourceNode) {
		return overlap->bID();
	}
	return overlap->aID();
}

