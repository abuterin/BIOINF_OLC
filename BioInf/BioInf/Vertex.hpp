//#pragma once
#ifndef VERTEX_H_
#define VERTEX_H_
#include "CommonHeaders.hpp"

using namespace std;

class Edge;

/**
Class created by Mirela
*/
class Vertex {
private:
	bool _marked = false;
public:
	string readString; //ocitanje, reads
	unsigned int readID;
	Read* read;
	vector<Edge*> edges_b; //preklapanja koja koriste početak očitanja
	vector<Edge*> edges_e; //preklapanja koja koriste kraj očitanja

	/*Vertex(string _read, unsigned int _readID) {
		readString = _read;
		readID = _readID;
	}*/

	Vertex(Read* _read, unsigned int _readID) : read{ _read }, readID{ _readID }, readString{ _read->read() } {}

	uint32_t getId() {
		return readID;
	}
	int getLength() {
		return readString.size();
	}
	string getSequence() {
		return read->read();
	}


	vector<Edge*>& getEdgesB();

	vector<Edge*>& getEdgesE();

	void addEdge(Edge* _edge);

	bool isTipCandidate();

	bool isBubbleRootCandidate(bool direction);

	Edge* bestEdge(bool useEnd);


	bool isBeginEdge(Edge* e);

	string getReverseComplement();

	double getCoverage() { return read->coverage(); }

	void removeMarkedEdges(bool propagate = true);

	void mark() { _marked = true; }

	bool isMarked() { return _marked; }
};
#endif //VERTEX_H_