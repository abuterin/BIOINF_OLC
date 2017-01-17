#pragma once
#include "CommonHeaders.hpp"

#include "edlib.h"
#include "Walk.hpp"
#include "Utils.hpp"

using namespace std;

class StringGraphComponent;
class StringGraphWalk;
class Walk;
class Edge;


#define ABS(x) ((x < 0) ? x * (-1) : x)
#define MAX(x,y) ((x > y) ? x : y)
#define MAX_STEPS 20
#define MAX_WALKS 20

int NOT_FOUND = -1;
int NOT_DEFINED = -1;

int MAX_NODES = 160;
int MAX_DISTANCE = MAX_NODES * 10000;
double MAX_DIFFERENCE = 0.25;


/**
Class created by Mirela & Ante
*/
/*!
* @details Creates a Graph object from reads and overlaps between them.
* Each Read becomes a Vertex and from each Overlap two Edges are created,
* one from A to B and one from B to A (A and B are reads in Overlap).
*/
class Graph {
public:
	map<unsigned int, Vertex*> vertices;//Nodes 
	vector<Edge*> edges;

	Graph(map<unsigned int, Read*> reads, vector<DovetailOverlap*> overlaps);
	
	~Graph();
	
	Vertex* getVertex(int id) {
		if (!vertices.count(id)) {
			return nullptr;
		}

		return vertices.at(id);
	}

	Edge* getEdgeById(unsigned int ID);

	Vertex* getVertexById(unsigned int vertexId);

	bool trim();

	unsigned int findBubbles(Vertex* startNode, bool direction);

	unsigned int popBubbles();

	unsigned int popBubble(vector<Walk*> walks, unsigned int junctionID, bool direction);
	/*!
	* @brief Method for graph simplification
	* @details Method calls both trimming and bubble popping in an alternating
	* fashion until no changes occur in graph.
	*/
	void simplify();

	//map<unsigned int, vector<Edge*>> Graph::extractingUnitigs();

	//void Graph::getEdges(vector<Edge*>* dst_edges, vector<unsigned int>* visitedNodes, Vertex* startNode, int startDirection);

	void extractComponents(vector<StringGraphComponent*>& dst); 
	

	int extractUnitigs(std::vector<StringGraphWalk*>* walks);

	int markUnitig(std::vector<Edge*>* dst_edges, std::vector<int>* unitig_id,
		int id, Vertex* start, int start_direction);

	static unsigned char toUnsignedChar(char c);

	int editDistance(const string& queryStr, const string& targetStr);

	void deleteMarked();

	void deleteMarkedEdges();

	void deleteMarkedVertices();

};
