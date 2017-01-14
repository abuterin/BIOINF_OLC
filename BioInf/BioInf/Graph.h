#pragma once
#include <iostream>
#include <vector>
#include "MHAPOverlap.hpp"

using namespace std;

class Edge {
public:
	unsigned int edgeId;
	MHAPOverlap* overlap; 
	unsigned int sourceNode;
	//unsigned int destinationNode;

	Edge(int _edgeId, MHAPOverlap _overlap, unsigned int _sourceNode) {
		cout << "Constructor called." << endl;
		edgeId = _edgeId;
		overlap = new MHAPOverlap(_overlap.aID, _overlap.bID, _overlap.jaccardScore, _overlap.sharedMinMers, _overlap.aForward, _overlap.aStart, _overlap.aEnd,
			_overlap.aLength, _overlap.bForward, _overlap.bStart, _overlap.bEnd, _overlap.bLength);
		sourceNode = _sourceNode;
	}
	~Edge() {
		delete overlap;
	}

};

class Vertex {
public:	
	string name; //očitanje, reads
	vector<Edge> edges_b; //preklapanja koja koriste početak očitanja
	vector<Edge> edges_e; //preklapanja koja koriste kraj očitanja

	Vertex(string _name){
		name = _name;
	}
	~Vertex(){}

	/*void addEdge(Edge _edge) {
		bool using_suffix = _edge.overlap.suffixA();
		if (using_suffix)
			edges_e.push_back(_edge);
		else
			edges_b.push_back(_edge);
	}*/

	//bool suffixA() { return _suffixA; }
	//bool suffixB() { return _suffixB; }
};

class Graph {
	vector<Vertex> vertices;
	vector<Edge> edges;
	Graph(vector<string> reads, vector<MHAPOverlap*> overlaps) {
		for (int i = 0; i < reads.size(); i++) {
			Vertex ver(reads[i]);
			vertices.push_back(ver);
		}
		for (int j = 0; j < overlaps.size; j++) {
			MHAPOverlap* o = overlaps[j];
			unsigned int source = o->aID;
			MHAPOverlap ov = new MHAPOverlap(overlaps[j]->aID, overlaps)
			int edgeId = j;
			Edge edge_a(edgeId,  , source);
			//Edge(int _edgeId, MHAPOverlap _overlap, unsigned int _sourceNode) {
		}
	}


};
