﻿#include "Graph.hpp"

Graph::Graph(map<unsigned int, Read*> reads, vector<DovetailOverlap*> overlaps) {

	//stvaranje čvorova
	map<unsigned int, Read*>::iterator it;
	for (it = reads.begin(); it != reads.end(); it++) {
		Vertex* ver = new Vertex(it->second, it->first);
		vertices[it->first] = ver;
	}
	//stvaranje bridova
	for (unsigned int j = 0; j < overlaps.size(); j++) {
		DovetailOverlap* ovp = overlaps[j];
		//Edge(int _edgeId, MHAPOverlap* _overlap, Vertex* src, Vertex* dst)
		
		Edge* edge_a = new Edge(edges.size(), ovp, ovp->aID(), this);
		edges.push_back(edge_a);

		Edge* edge_b = new Edge(edges.size(), ovp, ovp->bID(), this);
		edges.push_back(edge_b);

		vertices[ovp->aID()]->addEdge(edge_b);
		vertices[ovp->bID()]->addEdge(edge_a);

		edge_a->pairId = edge_b->edgeId;
		edge_b->pairId = edge_a->edgeId;
	}
	//Edge(uint32_t id, uint32_t readId, Overlap* overlap, StringGraph* graph);
	//Edge(edges_.size(), overlap->a(), overlap, this);
}
Graph::~Graph() {
	for (auto& vertex : vertices) delete vertex.second;
	for (auto& edge : edges) delete edge;
	vertices.clear();
	edges.clear();
}

Edge* Graph::getEdgeById(unsigned int ID) {
	for (int i = 0; i < edges.size(); i++) {
		if (edges[i]->edgeId == ID)
			return edges[i];
	}
	return nullptr;
}

Vertex* Graph::getVertexById(unsigned int vertexId) {
	if (!vertices.count(vertexId)) {
		return nullptr;
	}
	return vertices.at(vertexId);

}

bool Graph::trim() {	//as defined in (Vaser, 2015), page 23s
	vector<unsigned int> markedVertices;//razlikujemo ih po readID
	bool changes = false;//nismo još obrisali nijedan čvor

	map<unsigned int, Vertex*>::iterator it;
	for (it = vertices.begin(); it != vertices.end(); it++) {
		//it->first == IdRead, it->second == vertex
		int sizeEdges_b = (it->second)->edges_b.size();
		int sizeEdges_e = (it->second)->edges_e.size();
		//removingNodesWithNoEdges
		if (sizeEdges_b == 0 && sizeEdges_e == 0) {
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

vector<Walk*> Graph::findBubbles(Vertex* startNode, bool direction, int MAX_STEPS, int MAX_WALKS) {
	vector<Walk*> walks;
	vector<unsigned int, unsigned int> endsIn; //endsIn[i] num of paths ending in x
	vector<Edge*> _edges;
	unsigned int junctionID = NOT_FOUND;
	unsigned int bubblesPopped = 0;

	if (direction) {//direction == B
		_edges = startNode->edges_b;
	}
	else
	{
		_edges = startNode->edges_e;
	}

	walks.push_back(new Walk(startNode));

	for (size_t i = 0; i < MAX_STEPS; i++) {
		unsigned int deadWalks = 0; //counter
		unsigned int walksSize = walks.size();

		if (walksSize > MAX_WALKS) {
			break;
		}

		for (size_t j = 0; j < walksSize && junctionID == NOT_FOUND; j++) {
			Walk* walk = walks[j];
			vector<Walk*> extendedWalks = walk->extend(direction, this);

			if (extendedWalks.empty()) {		//current walk is a dead end
				deadWalks++;
				continue;
			}

			walk = extendedWalks.back();		//replace old walk with the extended one (which is at the end)

			for (size_t k = 0; k < extendedWalks.size() - 1; k++) {
				walks.push_back(extendedWalks[k]);	//add all new forked walks to the list
			}

			for (Walk* newWalk : extendedWalks) {
				endsIn[newWalk->lastNode()->readID]++;	//update counter for walk ends
				if (endsIn[newWalk->lastNode()->readID] == walks.size()) {
					junctionID = newWalk->lastNode()->readID;
					break;
				}
			}
		}

		if (deadWalks == walksSize) {		//all walks are dead (inside hihihi)
			break;
		}

		if (junctionID != NOT_FOUND) {
			cout << "Bubble root: " << startNode->readID << "\nJunction node: " << junctionID << endl;

			for (Walk* walk : walks) {
				walk->rewindTo(junctionID);
			}
			bubblesPopped += popBubble(walks, junctionID, direction);
			break; //break from the first 'for' because junction has surely been found
		}

	}//end of: for (size_t i = 0; i < MAX_STEPS; i++) {

	cout << "Bubbles popped: " << bubblesPopped << endl;

	for (Walk* walk : walks) {
		//ovo dovrsiti:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
		delete walk;
	}

	walks.clear();
	return walks;
}

bool Graph::bubbles() {	//ako je došlo do promjena vrati true
					//detect node with more than one outgoing edge
	map<unsigned int, Vertex*>::iterator it;
	for (it = vertices.begin(); it != vertices.end(); it++) {
		if ((it->second)->isBubbleRootCandidate(true)) {//direction==B
			//findBubbles(it->second, true, MAX_STEPS);
		}

	}

}

unsigned int Graph::popBubble(vector<Walk*> walks, unsigned int junctionID, bool direction) {
	double bestCoverage = 0;
	Walk* baseWalk;
	map<unsigned int, unsigned int> edgeUsed; //number of walks using certain edge

	auto getType = [](Edge* edge, unsigned int id) -> int {
		if (edge->overlap->aID() == id) return 0;
		if (!edge->overlap->isInnie()) return 0;
		return 1;
	};

	for (Walk* walk : walks) {
		for (Edge* e : walk->pathEdges()) {
			edgeUsed[e->edgeId]++;
		}
	}

	auto countExternalEdges = [&edgeUsed](Vertex* v, Edge* incomingEdge) -> int {
		int externalEdges = 0;

		vector<Edge*> vEdges = v->isBeginEdge(incomingEdge) ? v->edges_b : v->edges_e;
		for (Edge* edge : vEdges) {
			if (edgeUsed.count(edge->edgeId) == 0) {
				externalEdges++;
			}
		}

		return externalEdges;
	};

	for (Walk* walk : walks) {
		int externalEdgesBefore = 0;

		for (Edge* edge : walk->pathEdges()) {

			edgeUsed[edge->edgeId] += externalEdgesBefore;

			Vertex* v = vertices[edge->getDestinationNode()];
			int newExternalEdges = countExternalEdges(v, edge);
			edgeUsed[edge->edgeId] += newExternalEdges;

			externalEdgesBefore += newExternalEdges;
		}
	}

	vector<string> sequences;
	size_t selectedWalk = 0;
	double maxScore = 0;

	size_t i = 0;

	for (Walk* walk : walks) {

		double errate = 0;
		double coverage = 0;

		for (Edge* edge : walk->pathEdges()) {
			errate += edge->overlap->jaccardScore();
			coverage += edge->getDst()->getCoverage();

			coverage -= edge->overlap->coveredPercentageReadA();
			coverage -= edge->overlap->coveredPercentageReadB();
		}
		//if (walk->coverage(direction) > bestCoverage) {
		//	bestCoverage = walk->coverage(direction);
		//	baseWalk = walk;
		//}
		errate /= walk->pathEdges().size();

		double score = (1 - errate) * coverage;
		if (score > maxScore) {
			selectedWalk = i;
			maxScore = score;
		}
		i++;
	}

	for (Walk* walk : walks) {
		int startInverted = getType(walk->pathEdges().front(), walk->nodes().front()->readID);

		string sequence;
		walk->extractSequence(sequence);

		sequences.push_back(startInverted ? reverseComplement(sequence) : sequence);
	}

	bool anyPopped = false;

	for (size_t i = 0; i < sequences.size(); i++) {
		if (i == selectedWalk) {
			continue;
		}

		unsigned int smaller = min(sequences[i].size(), sequences[selectedWalk].size());
		unsigned int bigger = max(sequences[i].size(), sequences[selectedWalk].size());
		if ((bigger - smaller) / (double)bigger >= MAX_DIFFERENCE) {

		}
	}
}

void Graph::simplify() {
	//Timer timer;
	//timer.start();
	size_t numTrimmingRounds = 0;
	size_t numBubbleRounds = 0;
	size_t numVertices = 0;
	size_t numEdges = 0;
	while (numVertices != vertices.size() || numEdges != edges.size()) {
		fprintf(stderr, "new simplification round...\n");

		numVertices = vertices.size();
		numEdges = edges.size();
		// trimming
		size_t num_vertices_before = 0;
		while (num_vertices_before != vertices.size()) {

			num_vertices_before = vertices.size();

			++numTrimmingRounds;
			fprintf(stderr, "trimming...\n");
			trim();
		}
		// bubble popping
		size_t num_edges_before = edges.size();

		fprintf(stderr, "[SG][bubble popping]: max bucket size %lu\n", MAX_NODES);
		//popBubbles();

		++numBubbleRounds;

		if (num_vertices_before == vertices.size() && num_edges_before == edges.size()) {
			break;
		}
	}
	fprintf(stderr, "[SG][simplification]: number of trimming rounds = %zu\n", numTrimmingRounds);
	fprintf(stderr, "[SG][simplification]: number of bubble popping rounds = %zu\n", numBubbleRounds);

	//timer.stop();
	//timer.print("SG", "simplification");

}

/*
map<unsigned int, vector<Edge*>> Graph::extractingUnitigs() {
	vector<unsigned int> visitedNodes;
	map<unsigned int, vector<Edge*>> unitigs;//<start read,overlaps>

	for (auto it : vertices) {
		bool visited = false;
		Vertex* node = it.second;
		for (int i = 0; i < visitedNodes.size(); i++) {
			if (visitedNodes[i] == it.first) {
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
			Edge* pairId = this->getEdgeById(dst_edges[j]->pairId);
			dst_edges[j] = pairId;
		}
		getEdges(&dst_edges, &visitedNodes, node, 1);//direction_right
		unitigs[dst_edges[0]->sourceNode] = dst_edges;
	}
	return unitigs;
}

void Graph::getEdges(vector<Edge*>* dst_edges, vector<unsigned int>* visitedNodes, Vertex* startNode, int startDirection) {
	//int marked = 0;
	int useSuffix = startDirection;
	auto currentNode = startNode;
	unsigned int currentNodeId;
	while (true){
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
 */

void Graph::extractComponents(vector<StringGraphComponent*>& dst) {
	
	uint32_t maxId = 0;

	for (auto kv : vertices) {
		auto vertex = kv.second;
		maxId = max(vertex->getId(), maxId);
	}

	std::vector<bool> used(maxId + 1, false);

	for (auto kv : vertices) {
		auto vertex = kv.second;

		if (used[vertex->getId()] == true) {
			continue;
		}

		std::vector<int> expanded;
		expanded.emplace_back(vertex->getId());

		std::set<int> componentVertices;
		componentVertices.insert(vertex->getId());

		while (expanded.size() != 0) {

			std::vector<int> newExpanded;

			for (auto id : expanded) {

				auto eVertex = getVertex(id);

				for (auto edge : eVertex->getEdgesB()) {
					auto pair = componentVertices.insert(edge->getDst()->getId());

					if (pair.second == true) {
						newExpanded.emplace_back(edge->getDst()->getId());
					}
				}

				for (auto edge : eVertex->getEdgesE()) {
					auto pair = componentVertices.insert(edge->getDst()->getId());

					if (pair.second == true) {
						newExpanded.emplace_back(edge->getDst()->getId());
					}
				}
			}

			expanded.swap(newExpanded);
		}

		for (auto& id : componentVertices) {
			used[id] = true;
		}

		dst.emplace_back(new StringGraphComponent(componentVertices, this));
	}

	fprintf(stderr, "[SG][component extraction]: number of components = %zu\n", dst.size());

	//timer.stop();
	//timer.print("SG", "component extraction");
}


int Graph::extract_unitigs(std::vector<StringGraphWalk*>* walks) {

	uint32_t max_id = 0;
	for (auto kv : vertices) {
		max_id = max(kv.second->getId(), max_id);
	}

	// vertex_id -> unitig_id
	std::vector<int> unitig_id(max_id + 1, NOT_DEFINED);
	int curr_unitig_id = 1;

	for (auto kv : vertices) {

		auto vertex = kv.second;

		if (unitig_id[vertex->getId()] != NOT_DEFINED) continue;

		debug("UNITIGFIND %d ", vertex->getId());

		std::vector<Edge*> edges;

		// mark from vertex to the start of unitig
		mark_unitig(&edges, &unitig_id, curr_unitig_id, vertex, 0);

		// reverse edges
		for (int i = 0, n = edges.size(); i < n; ++i) {
			edges[i] = edges[i]->pair();
		}
		std::reverse(edges.begin(), edges.end());

		// mark from here to the end
		mark_unitig(&edges, &unitig_id, curr_unitig_id, vertex, 1);

		if (edges.size()) {

			walks->emplace_back(new StringGraphWalk(edges.front()->getSrc()));

			for (auto e : edges) {
				walks->back()->addEdge(e);
			}

			debug("UNITIGFOUND %d edges no %lu\n", vertex->getId(), edges.size());

			// create next unitig id
			curr_unitig_id++;
		}
	}

	return curr_unitig_id - 1;
}

int Graph::mark_unitig(std::vector<Edge*>* dst_edges, std::vector<int>* unitig_id,
	int id, Vertex* start, int start_direction) {

	int marked = 0;
	int use_suffix = start_direction;

	auto curr_vertex = start;

	while (true) {
		(*unitig_id)[curr_vertex->getId()] = id;

		marked++;

		auto edge = curr_vertex->bestEdge(use_suffix);

		if (edge == nullptr) {
			break;
		}

		if (edge->getOverlap()->is_innie()) {
			use_suffix = 1 - use_suffix;
		}

		auto next = edge->getDst();

		// if curr and next do not share best overlap
		if (next->bestEdge(1 - use_suffix)->getOverlap() != edge->getOverlap()) {
			break;
		}

		dst_edges->push_back(const_cast<Edge*>(edge));

		// if read is already part of some other unitig
		if (unitig_id->at(next->getId()) != NOT_DEFINED) {
			break;
		}

		curr_vertex = next;
	}

	return marked;
}