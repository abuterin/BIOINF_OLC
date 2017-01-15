#include "Graph.hpp"

Graph::Graph(map<unsigned int, Read*> reads, vector<MHAPOverlap*> overlaps) {
	//stvaranje �vorova
	map<unsigned int, Read*>::iterator it;
	for (it = reads.begin(); it != reads.end(); it++) {
		Vertex ver(it->second->read(), it->first);
		vertices[it->first] = ver;
	}
	//stvaranje bridova
	for (unsigned int j = 0; j < overlaps.size(); j++) {
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

Vertex Graph::getVertexById(unsigned int vertexId) {
	map<unsigned int, Vertex>::iterator it;
	for (it = vertices.begin(); it != vertices.end(); it++) {
		if (it->first == vertexId)
			return it->second;
	}
}

bool Graph::trim() { //as defined in (Vaser, 2015), page 23s
		vector<unsigned int> markedVertices;//razlikujemo ih po readID
		bool changes = false;//nismo jo� obrisali nijedan �vor

		map<unsigned int, Vertex>::iterator it;
		for (it = vertices.begin(); it != vertices.end(); it++) {
			//it->first == IdRead, it->second == vertex
			int sizeEdges_b = (it->second).edges_b.size();
			int sizeEdges_e = (it->second).edges_e.size();
			//removingNodesWithNoEdges
			if (sizeEdges_b == 0 && sizeEdges_e == 0) {
				markedVertices.push_back(it->first);
				continue;
			}
			//removingTrims
			if (sizeEdges_b == 0 || sizeEdges_e == 0) {
				vector<Edge> _edges;
				if (sizeEdges_b == 0 && sizeEdges_e != 0) {
					_edges = (it->second).edges_e;
				}
				if (sizeEdges_b != 0 && sizeEdges_e == 0) {
					_edges = (it->second).edges_b;
				}
				for (int i = 0; i < _edges.size(); i++) {
					//check if the opposing vertex has a similar edge
					unsigned int overtexId = _edges[i].opposite(it->first);//opposite vertex ID
					Vertex overtex = this->getVertexById(overtexId);
					vector<Edge> oedges = _edges[i].overlap->part(overtexId) == true ? overtex.edges_b : overtex.edges_e;
					bool isTip = false;
					for (int j = 0; j < oedges.size(); j++) {
						//vertex is a tip only if the vertex on the similar edge is not
						unsigned int oppID = oedges[j].opposite(overtexId);
						Vertex oppVertex = this->getVertexById(oppID);
						if (!oppVertex.isTipCandidate()) {
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

void Graph::findBubbles(Vertex startNode, bool direction, int MAX_STEPS, int MAX_WALKS) {
	vector<unsigned int> walks;
	vector<int> endsIn; //endsIn[i] num of paths ending in x
	vector<Edge> _edges;
	if (direction) {//direction == B
		_edges = startNode.edges_b;
	}
	else
	{
		_edges = startNode.edges_e;
	}
	walks.push_back(Walk(startNode));
	for (int i = 0; i < MAX_STEPS; i++) {
		int deadWalks = 0; //counter
		int walksSize = walks.size();
		if (walksSize > MAX_WALKS) {
			break;
		}
		for (int j = 0; j < walksSize; j++) {

		}
	}
}

bool Graph::bubbles() {
	//ako je do�lo do promjena vrati true
	//detect node with more than one outgoing edge
	map<unsigned int, Vertex>::iterator it;
	for (it = vertices.begin(); it != vertices.end(); it++) {
		if ((it->second).isBubbleRootCandidate(true)) {//direction==B
			findBubbles(it->second, true, MAX_STEPS);
		}

	}
}

void Graph::simplify() {
	bool graphChanges = true;
	while (graphChanges)
	{
		graphChanges = false;
		if (trim()) graphChanges = true;
		if (bubbles()) graphChanges = true;
	}
}
