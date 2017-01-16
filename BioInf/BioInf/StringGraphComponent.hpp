#pragma once
#include "MHAPOverlap.hpp"
#include "DovetailOverlap.hpp"
#include "Assembler.hpp"
#include "Read.hpp"
#include "Graph.h" //maknut kasnije
#include "Graph.hpp"
#include <iostream>
#include <vector>
#include <map>
#include "Vertex.hpp"
#include "StringGraphWalk.hpp"

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