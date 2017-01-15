/*#pragma once
#include <iostream>
#include <vector>
#include <map>
#include "MHAPOverlap.hpp"
#include "Assembler.hpp"
#include "Read.hpp"
#include "Vertex.hpp"

using namespace std;

class Graph {
public:
	map<unsigned int, Vertex> vertices;//Nodes
	vector<Edge> edges;
	Graph(map<unsigned int, Read*> reads, vector<MHAPOverlap*> overlaps);

	Vertex getVertexById(unsigned int vertexId);

	bool trim();

	void findBubbles(Vertex startNode, bool direction, int MAX_STEPS, int MAX_WALKS);

	bool bubbles();

	void simplify();
};*/