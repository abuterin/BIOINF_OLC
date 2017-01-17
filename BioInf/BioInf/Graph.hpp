#pragma once
#include "CommonHeaders.hpp"

using namespace std;

class Walk;

int MAX_NODES = 160;
int MAX_DISTANCE = MAX_NODES * 10000;
double MAX_DIFFERENCE = 0.25;

#define ABS(x) ((x < 0) ? x * (-1) : x)
#define MAX(x,y) ((x > y) ? x : y)

int NOT_FOUND = -1;
int NOT_DEFINED = -1;


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

	vector<Walk*> findBubbles(Vertex* startNode, bool direction, int MAX_STEPS, int MAX_WALKS);

	bool bubbles();

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
	

	int extract_unitigs(std::vector<StringGraphWalk*>* walks);

	int mark_unitig(std::vector<Edge*>* dst_edges, std::vector<int>* unitig_id,
		int id, Vertex* start, int start_direction);

};
