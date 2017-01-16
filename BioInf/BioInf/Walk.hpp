#pragma once
#include<vector>
#include "Graph.h"		//change to Vertex.hpp

using namespace std;

class Vertex;
/*
	Class created by Ante
*/
class Walk {
private:
	vector<Vertex*> _nodes; //nodes in a path/walk
	vector<Edge*> _pathEdges;
	Vertex* _lastNode;		//for checking wheter sink has been found
	double _coverage;
public:
	Walk(Vertex* startNode) : _lastNode{ startNode }, _coverage{ 0 } {
		_nodes.push_back(startNode);
	}
	Walk(Walk* walk) : _coverage{ 0 } {	//Walk created from forking parental Walk
		_nodes = walk->nodes();
		_lastNode = walk->lastNode();
		_pathEdges = walk->pathEdges();
	}

	Vertex* lastNode() { return _lastNode; }
	vector<Vertex*> nodes() { return _nodes; }
	vector<Edge*> pathEdges() { return _pathEdges; }

	void addNode(Vertex* newNode);

	vector<Walk*> extend(bool direction, Graph*); //true if extending has occured, false otherwise

	void rewindTo(unsigned int);

	double coverage(bool);
};