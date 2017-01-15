#pragma once
#include <iostream>
#include <vector>
#include <map>
#include "MHAPOverlap.hpp"
#include "Assembler.hpp"
#include "Read.hpp"
#include "Edge.hpp"

using namespace std;

class Vertex {
public:
	string readString; //oèitanje, reads
	unsigned int readID;
	vector<Edge> edges_b; //preklapanja koja koriste poèetak oèitanja
	vector<Edge> edges_e; //preklapanja koja koriste kraj oèitanja

	Vertex(string _read, unsigned int _readID) : readString { _read }, readID{ _readID }{}

	void addEdge(Edge _edge);
	bool isTipCandidate();
	bool isBubbleRootCandidate(bool direction);
};