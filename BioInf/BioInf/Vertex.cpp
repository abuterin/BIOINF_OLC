#include "Vertex.hpp"

list<Edge*>& Vertex::getEdgesB() {
	list<Edge*> edgesB;
	for (auto& it : edges_b) {
		edgesB.push_back(it);
	}
	return edgesB;
}
list<Edge*>& Vertex::getEdgesE() {
	list<Edge*> edgesE;
	for (auto& it : edges_e) {
		edgesE.push_back(it);
	}
	return edgesE;
}
void Vertex::addEdge(Edge* _edge) {
	bool using_suffix = _edge->overlap->isUsingSuffix(this->readID);
	if (using_suffix)
		edges_e.push_back(_edge);//bridovi koji koriste readov kraj
	else
		edges_b.push_back(_edge);
}
bool Vertex::isTipCandidate() {
	if (edges_b.size() == 0 || edges_e.size() == 0)	return true;
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
}
unsigned int Vertex::bestEdge(bool useEnd) { //bool useSuffix = startDirection;
	auto edges = useEnd ? edges_e : edges_b;
	if (edges.size() == 0) {
		return -1;//edge id koji ne postoji
	}
	Edge* bestEdge = edges.front();//prvi element u vektoru
	int bestLength = bestEdge->overlap->getLength(readID);//dužina očitanja u preklapanju
	for (int i = 1; i < edges.size(); i++) {
		int currLength = edges[i]->overlap->getLength(readID);
		if (currLength > bestLength) {
			bestEdge = edges[i];
			bestLength = currLength;
		}
	}
	return bestEdge->edgeId;
}

bool Vertex::isBeginEdge(Edge* e) {
	if (e == nullptr) {
		return false;
	}
	if (e->overlap->isUsingSuffix(readID)) {
		return false;
	}
	return true;
}

string Vertex::getReverseComplement() {
	return read->reverseComplement();
}
