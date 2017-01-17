#include "StringGraphWalk.hpp"

StringGraphWalk::StringGraphWalk(Vertex* start) :start_(start) {
}

void StringGraphWalk::addEdge(Edge* edge) {

	edges_.emplace_back(edge);
	visitedVertices_.insert(edge->getDst()->getId());
	visitedEdges_.insert(edge->edgeId);
}

void StringGraphWalk::extractSequence(std::string& dst) {

	if (edges_.empty()) {
		dst = std::string(start_->getSequence());
		return;
	}

	// types: 0 - normal, 1 - reverse complement
	auto getType = [](Edge* edge, uint32_t id) -> int {
		if (edge->overlap->aID() == id) return 0; // due to possible overlap types
		if (!edge->overlap->isInnie()) return 0;
		return 1;
	};

	int startType = getType(edges_.front(), start_->getId());

	bool appendToPrefix = edges_.front()->overlap->is_using_prefix(start_->readID) ^ startType;

	std::string startSequence = std::string(startType ? start_->getReverseComplement() : start_->getSequence());

	// add start vertex
	dst = appendToPrefix ? std::string(startSequence.rbegin(), startSequence.rend()) : startSequence;

	int prevType = startType;

	// add edge labels
	for (auto& edge : edges_) {

		int type = getType(edge, edge->getSrc()->getId());

		bool invert = type == prevType ? false : true;

		std::string label;
		if (invert) {
			edge->rkLabel(label);
		}
		else {
			edge->label(label);
		}

		dst += appendToPrefix ? std::string(label.rbegin(), label.rend()) : label;

		prevType = getType(edge, edge->getDst()->getId()) ^ invert;
	}

	if (appendToPrefix) dst = std::string(dst.rbegin(), dst.rend());
}

void StringGraphWalk::extractVertices(std::vector<Vertex*>& dst) {
	// add start vertex
	dst.push_back(start_);

	// add edge labels
	for (auto& edge : edges_) {
		dst.push_back(edge->getDst());
	}
}

bool StringGraphWalk::containsVertex(int id) {
	return visitedVertices_.count(id) > 0;
}

bool StringGraphWalk::containsEdge(int id) {
	return visitedEdges_.count(id) > 0;
}