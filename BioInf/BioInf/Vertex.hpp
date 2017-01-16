#pragma once
#include "CommonHeaders.hpp"

using namespace std;


/**
Class created by Mirela
*/
class Vertex {
public:
	string readString; //ocitanje, reads
	unsigned int readID;
	vector<Edge*> edges_b; //preklapanja koja koriste početak očitanja
	vector<Edge*> edges_e; //preklapanja koja koriste kraj očitanja

	Vertex(string _read, unsigned int _readID) {
		readString = _read;
		readID = _readID;
	}

	uint32_t getId() {
		return readID;
	}

	list<Edge*>& getEdgesB();

	list<Edge*>& getEdgesE();

	void addEdge(Edge* _edge);

	bool isTipCandidate();

	bool isBubbleRootCandidate(bool direction);

	unsigned int bestEdge(bool useEnd);


	bool isBeginEdge(Edge* e);
};