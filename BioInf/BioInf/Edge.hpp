/*#pragma once
#include <iostream>
#include <vector>
#include <map>
#include "MHAPOverlap.hpp"
#include "Assembler.hpp"
#include "Read.hpp"
using namespace std;

class Edge {
public:
	int edgeId;
	MHAPOverlap* overlap;
	unsigned int sourceNode;
	int pair;
	//unsigned int destinationNode;
	Edge(int _edgeId, MHAPOverlap* _overlap, unsigned int _sourceNode) : edgeId{ _edgeId }, overlap{ _overlap }, sourceNode{ _sourceNode } {
		cout << "Constructor called." << endl;
	}
	unsigned int opposite(unsigned int vertexID);
};*/
