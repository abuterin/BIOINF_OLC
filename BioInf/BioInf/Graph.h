#pragma once
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
	Edge(int _edgeId, MHAPOverlap* _overlap, unsigned int _sourceNode) {
		cout << "Constructor called." << endl;
		edgeId = _edgeId;
		overlap = _overlap;
		sourceNode = _sourceNode;
	}
};

class Vertex {
public:	
	string readString; //očitanje, reads
	unsigned int readID; 
	vector<Edge> edges_b; //preklapanja koja koriste početak očitanja
	vector<Edge> edges_e; //preklapanja koja koriste kraj očitanja

	Vertex(string _read, unsigned int _readID){
		readString = _read;
		readID = _readID;
	}
	void addEdge(Edge _edge) {
		bool using_suffix = _edge.overlap.isUsingSuffix(this->readID);
		if (using_suffix)
			edges_e.push_back(_edge);//bridovi koji koriste readov kraj
		else
			edges_b.push_back(_edge);
	}
};

class Graph {
public:
	map<unsigned int, Vertex> vertices;
	vector<Edge> edges;
	Graph(vector<Read*> reads, vector<MHAPOverlap*> overlaps) {
		//stvaranje čvorova
		for (int i = 0; i < reads.size(); i++) {
			Vertex ver(reads[i]->read(), reads[i]->ID()); //stvori čvor koji sadrži string i ID
			//vertices.push_back(ver);
			vertices[reads[i]->ID()] = ver;
		}
		//stvaranje bridova
		for (unsigned int j = 0; j < overlaps.size; j++) {
			MHAPOverlap* ovp = overlaps[j];

			Edge edge_a(edges.size(), ovp, ovp->aID());
			edges.push_back(edge_a);

			Edge edge_b(edges.size(), ovp, ovp->bID());
			edges.push_back(edge_b);

			vertices[ovp->aID()].addEdge(edge_b);
			vertices[ovp->bID()].addEdge(edge_a);
			
			edge_a.pair = edge_b.edgeId;
			edge_b.pair = edge_a.edgeId;
		}
	}


};