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
	Vertex* _lastNode;		//for checking wheter sink has been found
public:
	Walk(Vertex* startNode) : _lastNode{ startNode } {
		_nodes.push_back(startNode);
	}
	Walk(Walk* walk) {	//Walk created from forking parental Walk
		_nodes = walk->nodes();
		_lastNode = walk->lastNode();
	}

	Vertex* lastNode() { return _lastNode; }
	vector<Vertex*> nodes() { return _nodes; }

	void addNode(Vertex* newNode) { _nodes.push_back(newNode); _lastNode = newNode; }

	vector<Walk*> extend(bool direction, Graph*); //true if extending has occured, false otherwise

};