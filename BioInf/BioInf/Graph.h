#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include "MHAPOverlap.hpp"
#include "Assembler.hpp"
#include "Read.hpp"
#include "Walk.hpp"


#define ABS(x) ((x < 0) ? x * (-1) : x)
#define MAX(x,y) ((x > y) ? x : y)

int NOT_FOUND = -1;
int NOT_DEFINED = -1;

using namespace std;
/**
Class created by Mirela
*/
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
	unsigned int opposite(unsigned int vertexID) {
		if (overlap->aID() == vertexID) {
			return overlap->bID();
		}
		else if (overlap->bID() == vertexID) {
			return overlap->aID();
		}
	}
	unsigned int getDestinationNode() {
		unsigned int dstNode;
		if (overlap->aID() == sourceNode) {
			return overlap->bID();
		}
		return overlap->aID();
	}
};
/**
Class created by Mirela
*/
class Vertex {
public:	
	string readString; //očitanje, reads
	unsigned int readID; 
	vector<Edge*> edges_b; //preklapanja koja koriste početak očitanja
	vector<Edge*> edges_e; //preklapanja koja koriste kraj očitanja

	Vertex(string _read, unsigned int _readID){
		readString = _read;
		readID = _readID;
	}
	void addEdge(Edge* _edge) {
		bool using_suffix = _edge->overlap->isUsingSuffix(this->readID);
		if (using_suffix)
			edges_e.push_back(_edge);//bridovi koji koriste readov kraj
		else
			edges_b.push_back(_edge);
	}
	bool isTipCandidate() {
		if (edges_b.size() == 0 || edges_e.size() == 0)	return true;
		return false;
	}
	bool isBubbleRootCandidate(bool direction) {
		if (direction == true && edges_b.size() > 1) {//direction == B
			return true;
		}
		if (direction == false && edges_e.size() > 1) {//direction == E
			return true;
		}
		return false;
	}
	unsigned int bestEdge(bool useEnd) { //bool useSuffix = startDirection;
		auto edges = useEnd ? edges_e : edges_b;
		if (edges.size() == 0) {
			return -1;//edge id koji ne postoji
		}
		Edge* bestEdge = edges.front();//prvi element u vektoru
		int bestLength = bestEdge->overlap->getLength(readID);//dužina očitanja u preklapanju
		for (int i = 1; i < edges.size(); i++) {
			int currLength = edges[i]->overlap->getLength(readID);
			if (currLength > bestLength) {
				bestEdge = edges[i];
				bestLength = currLength;
			}
		}
		return bestEdge->edgeId;
	}
};
/**
Class created by Mirela
*/
class Graph {
public:
	map<unsigned int, Vertex*> vertices;//Nodes
	vector<Edge*> edges;
	Graph(map<unsigned int, Read*> reads, vector<MHAPOverlap*> overlaps) {
		//stvaranje čvorova
		map<unsigned int, Read*>::iterator it;
		for (it = reads.begin(); it != reads.end(); it++) {
			Vertex* ver = new Vertex(it->second->read(), it->first);
			vertices[it->first] = ver;
		}
		//stvaranje bridova
		for (unsigned int j = 0; j < overlaps.size(); j++) {
			MHAPOverlap* ovp = overlaps[j];

			Edge* edge_a = new Edge(edges.size(), ovp, ovp->aID());
			edges.push_back(edge_a);

			Edge* edge_b = new Edge(edges.size(), ovp, ovp->bID());
			edges.push_back(edge_b);

			vertices[ovp->aID()]->addEdge(edge_b);
			vertices[ovp->bID()]->addEdge(edge_a);
			
			edge_a->pair = edge_b->edgeId;
			edge_b->pair = edge_a->edgeId;
		}
	}
	Edge* getEdgeById(unsigned int ID) {
		for (int i = 0; i < edges.size(); i++) {
			if (edges[i]->edgeId == ID)
				return edges[i];
		}
		return nullptr;
	}

	Vertex* getVertexById(unsigned int vertexId) {
		map<unsigned int, Vertex*>::iterator it;
		for (it = vertices.begin(); it != vertices.end(); it++) {
			if (it->first == vertexId)
				return (it->second);
		}
	}


	bool trim() {	//as defined in (Vaser, 2015), page 23s
		vector<unsigned int> markedVertices;//razlikujemo ih po readID
		bool changes = false;//nismo još obrisali nijedan čvor

		map<unsigned int, Vertex*>::iterator it;
		for (it = vertices.begin(); it != vertices.end(); it++) {
			//it->first == IdRead, it->second == vertex
			int sizeEdges_b = (it->second)->edges_b.size();
			int sizeEdges_e = (it->second)->edges_e.size();
			//removingNodesWithNoEdges
			if ( sizeEdges_b==0 && sizeEdges_e==0 ) { 
				markedVertices.push_back(it->first);
				continue;
			}
			//removingTrims
			if (sizeEdges_b == 0 || sizeEdges_e == 0) {
				vector<Edge*> _edges;
				if (sizeEdges_b == 0 && sizeEdges_e != 0) {
					_edges = (it->second)->edges_e;
				}
				if (sizeEdges_b != 0 && sizeEdges_e == 0) {
					_edges = (it->second)->edges_b;
				}
				for (int i = 0; i < _edges.size(); i++) {
					//check if the opposing vertex has a similar edge
					unsigned int overtexId = _edges[i]->opposite(it->first);//opposite vertex ID
					Vertex* overtex = this->getVertexById(overtexId);
					vector<Edge*> oedges = _edges[i]->overlap->part(overtexId) == true ? overtex->edges_b : overtex->edges_e;
					bool isTip = false;
					for (int j = 0; j < oedges.size(); j++) {
						//vertex is a tip only if the vertex on the similar edge is not
						unsigned int oppID = oedges[j]->opposite(overtexId);
						Vertex* oppVertex = this->getVertexById(oppID);
						if (!oppVertex->isTipCandidate()) {
							isTip = true;
							break;
						}
					}
					if (isTip) {
						markedVertices.push_back(it->first);
					}
				}
			}
				

		}

		if (markedVertices.size() != 0) {
			changes = true;
			for (int i = 0; i < markedVertices.size(); i++) {
				vertices.erase(markedVertices[i]);
			}
		}
		return changes;
	
	}

	void findBubbles(Vertex* startNode, bool direction, int MAX_STEPS, int MAX_WALKS) {
		vector<Walk*> walks;
		vector<unsigned int> endsIn; //endsIn[i] num of paths ending in x
		vector<Edge*> _edges;
		if (direction) {//direction == B
			_edges = startNode->edges_b;
		}else
		{
			_edges = startNode->edges_e;
		}
		walks.push_back(new Walk(startNode));
		for (int i = 0; i < MAX_STEPS; i++) {
			unsigned int deadWalks = 0; //counter
			int walksSize = walks.size();
			if (walksSize > MAX_WALKS) {
				break;
			}
			for (int j = 0; j < walksSize; j++) {
				Walk* walk = walks[j];

			}
		}
	}

	bool bubbles() {	//ako je došlo do promjena vrati true
		//detect node with more than one outgoing edge
		map<unsigned int, Vertex*>::iterator it;
		for (it = vertices.begin(); it != vertices.end(); it++) {
			if ((it->second)->isBubbleRootCandidate(true)) {//direction==B
				findBubbles(it->second, true, MAX_STEPS);
			}

		}
		
	}
	
	void simplify() {
		bool graphChanges = true;
		while (graphChanges)
		{
			graphChanges = false;
			if (trim()) {
				graphChanges = true;
			}
			if (bubbles()) {
				graphChanges = true;
			}
		}
	}

	map<unsigned int, vector<Edge*>> extractingUnitigs() {
		vector<unsigned int> visitedNodes;
		map<unsigned int, vector<Edge*>> unitigs;//<start read,overlaps>
		
		map<unsigned int, Vertex*>::iterator it;
		for (it = vertices.begin(); it != vertices.end(); it++) {
			bool visited = false;
			Vertex* node = it->second;
			for (int i = 0; i < visitedNodes.size(); i++) {
				if (visitedNodes[i] == it->first) {
					visited = true;
					break;
				}
			}
			if (visited) {//if node is already part of some known unitig
				continue;
			}
			//if not, it starts buildinga new one
			vector<Edge*> dst_edges;
			getEdges(&dst_edges, &visitedNodes, node, 0);//direction_left
			
			//Reverse edges because we will return unitig going in other direction
			for (int j = 0; j < dst_edges.size(); j++) {
				Edge* pair = this->getEdgeById(dst_edges[j]->pair);
				dst_edges[j] = pair;
			}
			getEdges(&dst_edges, &visitedNodes, node, 1);//direction_right
			unitigs[dst_edges[0]->sourceNode] = dst_edges;
		}
		return unitigs;
	}

	void getEdges(vector<Edge*>* dst_edges, vector<unsigned int>* visitedNodes, Vertex* startNode, int startDirection) {
		//int marked = 0;
		int useSuffix = startDirection;
		auto currentNode = startNode;
		unsigned int currentNodeId;
		while (true)
		{
			currentNodeId = currentNode->readID;
			visitedNodes->push_back(currentNodeId);
			//marked++;
			unsigned int edgeId = currentNode->bestEdge(useSuffix);
			if (edgeId == -1) {
				break;
			}
			Edge* edge = this->getEdgeById(edgeId);
			if (edge->overlap->isInnie()) {//change direction if overlap is SS or PP
				useSuffix = 1 - useSuffix;
			}
			unsigned int nextNodeId = edge->getDestinationNode();
			Vertex* nextNode = this->getVertexById(nextNodeId);
			unsigned int nextBestEdgeId = nextNode->bestEdge(1 - useSuffix);
			Edge* nextBestEdge = this->getEdgeById(nextBestEdgeId);
			// if curr and next do not share best overlap
			if (nextBestEdge->overlap != edge->overlap) {
				break;
			}
			dst_edges->push_back(const_cast<Edge*>(edge));
			// if read is already part of some other unitig
			if (visitedNodes->at(nextNodeId) != NOT_DEFINED) {
				break;
			}
			currentNode = nextNode;
		}
		return;
		//return marked;
	}

	void extractingContigs() {
		vector<unsigned int> startCandidates;
		startCandidates = findStartCandidates();
	}
	vector<unsigned int> findStartCandidates() {

	}
	void extractLongestWalk() {
		typedef tuple<Vertex*, int, double> Candidate;
		// pick n start vertices based on total coverage of their chains to first branch
		vector<Candidate> startCandidates;
		uint32_t maxId = 0;
		for (auto& vertex : vertices) {
			maxId = MAX(maxId, vertex.first);
		}
		// tips and singular chains could be good candidates
		for (int direction = 0; direction <= 1; ++direction) {
			for (auto& vertex : vertices) {
				if ((direction == 0 && vertex.second->edges_b.size() == 1 && vertex.second->edges_e.size() == 0) ||
					(direction == 1 && vertex.second->edges_b.size() == 1 && vertex.second->edges_e.size() == 0)) {
					vector<bool> visited(maxId + 1, false);
					startCandidates.emplace_back(vertex, direction, longest_sequence_length(vertex, direction,
						visited, 0));
				}
			}
		}
		// forks could be good candidates, too
		for (int direction = 0; direction <= 1; ++direction) {
			for (auto& vertex : vertices) {
				if ((direction == 0 && vertex.second->edges_b.size() > 1) ||
					(direction == 1 && vertex.second->edges_e.size() > 1)) {
					vector<bool> visited(maxId + 1, false);
					startCandidates.emplace_back(vertex, direction, longest_sequence_length(vertex, direction,
						visited, 1));
				}
			}
		}
		// circular component
		if (startCandidates.size() == 0) {
			vector<bool> visited(maxId + 1, false);

			int direction = 0;
			auto vertex = vertices.begin;
			startCandidates.emplace_back(vertex, direction, longest_sequence_length(vertex, direction,
				visited, 1));
		}


	}
};