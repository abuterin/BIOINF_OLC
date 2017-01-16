#pragma once
#include "CommonHeaders.hpp"

using namespace std;
/**
Class created by Mirela
*/
class StringGraphWalk {
private:
	Vertex* start_;
	std::vector<Edge*> edges_;
	std::set<int> visitedVertices_;
	std::set<int> visitedEdges_;

public:
	StringGraphWalk(Vertex* start);

	~StringGraphWalk() {};

	Vertex* getStart() {
		return start_;
	}

	std::vector<Edge*>& getEdges() {
		return edges_;
	}

	/*!
	* @brief Method for path extension
	* @details Adds edge to end of path
	*/
	void addEdge(Edge* edge);

	/*!
	* @brief Method for sequence extraction
	* @details Method returns the sequence extracted from the path.
	* It equals the sequence from start vertex plus all edge labels.
	*
	* @param [out] dst sequence string
	*/
	void extractSequence(std::string& dst);

	/*!
	* @brief Method for vertices extraction
	* @details Method appends vertices ids extracted from the path.
	*
	* @param [out] dst vertices
	*/
	void extractVertices(std::vector<Vertex*>& dst);

	/*!
	* @brief Method for vertex check
	* @details Cheks whether a vertex with given identifier exists in path.
	*
	* @param [in] id vertex identifier
	* @return true if the vertex is in path
	*/
	bool containsVertex(int id);

	/*!
	* @brief Method for edge check
	* @details Cheks whether a edge with given identifier exists in path.
	*
	* @param [in] id edge identifier
	* @return true if the edge is in path
	*/
	bool containsEdge(int id);
};
