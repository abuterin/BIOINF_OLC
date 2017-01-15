/*#include "Vertex.hpp"

void Vertex::addEdge(Edge _edge) {
	bool using_suffix = _edge.overlap->isUsingSuffix(this->readID);
	if (using_suffix)
		edges_e.push_back(_edge);//bridovi koji koriste readov kraj
	else
		edges_b.push_back(_edge);
}

bool Vertex::isTipCandidate() {
	if (edges_b.size() == 0 || edges_e.size() == 0)
		return true;
	return false;
}

bool Vertex::isBubbleRootCandidate(bool direction) {
	if (direction == true && edges_b.size() > 1) {//direction == B
		return true;
	}
	if (direction == false && edges_e.size() > 1) {//direction == E
		return true;
	}
	return false;
}*/
