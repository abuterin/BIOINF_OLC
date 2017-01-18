//#pragma once
#ifndef GRAPH_COMPONENT_H_
#define GRAPH_COMPONENT_H_
#include "CommonHeaders.hpp"

#include "Edge.hpp"
#include "Vertex.hpp"
#include "StringGraphWalk.hpp"

class Graph;

using namespace std;

class StringGraphComponent {
private:
	vector<Vertex*> vertices_;
	Graph* graph_;
	StringGraphWalk* walk_;

	void extractLongestWalk();

public:
	/*!
	* @details Creates a StringGraphComponent object from graph by picking out
	* vertices with given identifiers.
	*/
	StringGraphComponent(set<int> vertexIds, Graph* graph);
	~StringGraphComponent();
	/*!
	* @details Calls extractLongestWalk if nullptr is store in walk_ and extracts
	* the sequence from walk_.
	*/
	void extractSequence(string& dst);

	StringGraphWalk* longestWalk();

	vector<Vertex*>& vertices() {
		return vertices_;
	}
};
#endif //GRAPH_COMPONENT_H_