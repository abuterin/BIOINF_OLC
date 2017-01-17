#include "Graph.hpp"
#include "Edge.hpp"
#include "StringGraphComponent.hpp"
#include "StringGraphWalk.hpp"

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
		edge_a->_pair = edge_b;
		edge_b->pairId = edge_a->edgeId;
		edge_a->_pair = edge_b;
	}
	//Edge(uint32_t id, uint32_t readId, Overlap* overlap, StringGraph* graph);
	//Edge(edges_.size(), overlap->a(), overlap, this);
}
Graph::~Graph() {
	map<unsigned int, Vertex*>::iterator it;
	for (it = vertices.begin(); it != vertices.end(); it++) delete it->second;
	for (Edge* edge : edges) delete edge;
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

unsigned int Graph::findBubbles(Vertex* startNode, bool direction) {
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
		for (Edge* edge : walk->pathEdges()) {
			edge->setInWalk(false);
			edge->pair()->setInWalk(false);
		}
		delete walk;
	}

	walks.clear();
	return bubblesPopped;
}

unsigned int Graph::popBubbles() {	//ako je došlo do promjena vrati true
					//detect node with more than one outgoing edge
	size_t bubblesPopped = 0;

	map<unsigned int, Vertex*>::iterator it;
	for (it = vertices.begin(); it != vertices.end(); it++) {
		Vertex* vertex = it->second;

		if (vertex->isMarked()) {
			continue;
		}

		for (int direction = 0; direction <= 1; direction++) {
			if (!vertex->isBubbleRootCandidate(direction)) {
				continue;
			}

			bubblesPopped += findBubbles(vertex, direction);
		}
	}

	if (bubblesPopped > 0) {
		deleteMarked();
	}

	return bubblesPopped;
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

	auto edgeKey = [](Edge* edge) -> unsigned int {
		return min(edge->edgeId, edge->pairId);
	};

	for (Walk* walk : walks) {
		for (Edge* e : walk->pathEdges()) {
			edgeUsed[edgeKey(e)]++;
		}
	}

	auto countExternalEdges = [&edgeUsed, &edgeKey](Vertex* v, Edge* incomingEdge) -> int {
		int externalEdges = 0;

		vector<Edge*> vEdges = v->isBeginEdge(incomingEdge) ? v->edges_b : v->edges_e;
		for (Edge* edge : vEdges) {
			if (edgeUsed.count(edgeKey(edge)) == 0) {
				externalEdges++;
			}
		}

		return externalEdges;
	};

	for (Walk* walk : walks) {
		int externalEdgesBefore = 0;

		for (Edge* edge : walk->pathEdges()) {

			unsigned int key = edgeKey(edge);
			edgeUsed[key] += externalEdgesBefore;

			Vertex* v = edge->getDst();
			int newExternalEdges = countExternalEdges(v, edge);
			edgeUsed[key] += newExternalEdges;

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
			continue;
		}

		int distance = editDistance(sequences[i], sequences[selectedWalk]);
		if (distance / (double)sequences[selectedWalk].size() >= MAX_DIFFERENCE) {
			continue;
		}

		bool popped = false;
		vector<Edge*> _edges = walks[i]->pathEdges();
		for (Edge* edge : _edges) {
			unsigned int key = edgeKey(edge);
			edgeUsed[key]--;

			if (edgeUsed[key] == 0) {
				edge->mark();
				edge->pair()->mark();

				popped = true;
			}
		}
		/*if (popped) {
			Walk* mainWalk = walks[selectedWalk];
			for (Edge* e : mainWalk->pathEdges) {
				//e->overlap->a
			}
		}*/
		anyPopped |= popped;
	}
	return anyPopped;
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
		popBubbles();

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
	map<unsigned int, Vertex*>::iterator it;
	for (it = vertices.begin(); it != vertices.end(); it++) {
		Vertex* vertex = it->second;
		maxId = max(vertex->getId(), maxId);
	}

	std::vector<bool> used(maxId + 1, false);

	for (it = vertices.begin(); it != vertices.end(); it++) {
		Vertex* vertex = it->second;

		if (used[vertex->getId()] == true) {
			continue;
		}

		std::vector<int> expanded;
		expanded.emplace_back(vertex->getId());

		std::set<int> componentVertices;
		componentVertices.insert(vertex->getId());

		while (expanded.size() != 0) {

			std::vector<int> newExpanded;

			for (int id : expanded) {

				Vertex* eVertex = getVertex(id);

				for (Edge* edge : eVertex->getEdgesB()) {
					auto pair = componentVertices.insert(edge->getDst()->getId());

					if (pair.second == true) {
						newExpanded.emplace_back(edge->getDst()->getId());
					}
				}

				for (Edge* edge : eVertex->getEdgesE()) {
					auto pair = componentVertices.insert(edge->getDst()->getId());

					if (pair.second == true) {
						newExpanded.emplace_back(edge->getDst()->getId());
					}
				}
			}

			expanded.swap(newExpanded);
		}

		for (int id : componentVertices) {
			used[id] = true;
		}

		dst.emplace_back(new StringGraphComponent(componentVertices, this));
	}

	fprintf(stderr, "[SG][component extraction]: number of components = %zu\n", dst.size());

	//timer.stop();
	//timer.print("SG", "component extraction");
}


int Graph::extractUnitigs(std::vector<StringGraphWalk*> walks) {

	uint32_t max_id = 0;
	map<unsigned int, Vertex*>::iterator it;
	for (it = vertices.begin(); it != vertices.end(); it++) {
		max_id = max(it->second->getId(), max_id);
	}

	// vertex_id -> unitig_id
	std::vector<int> unitig_id(max_id + 1, NOT_DEFINED);
	int curr_unitig_id = 1;

	for (it = vertices.begin(); it != vertices.end(); it++) {

		Vertex* vertex = it->second;

		if (unitig_id[vertex->getId()] != NOT_DEFINED) continue;

//		debug("UNITIGFIND %d ", vertex->getId());

		std::vector<Edge*> edges;

		// mark from vertex to the start of unitig
		markUnitig(&edges, &unitig_id, curr_unitig_id, vertex, 0);

		// reverse edges
		for (int i = 0, n = edges.size(); i < n; ++i) {
			edges[i] = edges[i]->pair();
		}
		std::reverse(edges.begin(), edges.end());

		// mark from here to the end
		markUnitig(&edges, &unitig_id, curr_unitig_id, vertex, 1);

		if (edges.size()) {

			walks.emplace_back(new StringGraphWalk(edges.front()->getSrc()));

			for (Edge* e : edges) {
				walks.back()->addEdge(e);
			}

//			debug("UNITIGFOUND %d edges no %lu\n", vertex->getId(), edges.size());

			// create next unitig id
			curr_unitig_id++;
		}
	}

	return curr_unitig_id - 1;
}

int Graph::markUnitig(std::vector<Edge*>* dst_edges, std::vector<int>* unitig_id,
	int id, Vertex* start, int start_direction) {

	int marked = 0;
	int use_suffix = start_direction;

	Vertex* curr_vertex = start;

	while (true) {
		(*unitig_id)[curr_vertex->getId()] = id;

		marked++;

		Edge* edge = curr_vertex->bestEdge(use_suffix);

		if (edge == nullptr) {
			break;
		}

		if (edge->getOverlap()->isInnie()) {
			use_suffix = 1 - use_suffix;
		}

		Vertex* next = edge->getDst();

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

unsigned char Graph::toUnsignedChar(char c) {
	unsigned char r;

	switch (c) {
	case 'A':
		r = 0;
		break;
	case 'T':
		r = 1;
		break;
	case 'G':
		r = 2;
		break;
	case 'C':
		r = 3;
		break;
	default:
		r = 4;
		break;
	}

	return r;
}

int Graph::editDistance(const string & queryStr, const string & targetStr) {
	if (queryStr.size() == 0) return targetStr.size();
	if (targetStr.size() == 0) return queryStr.size();

	unsigned char* query = new unsigned char[queryStr.size()];
	int queryLength = queryStr.size();

	for (int i = 0; i < queryLength; ++i) {
		query[i] = toUnsignedChar(queryStr[i]);
	}

	unsigned char* target = new unsigned char[targetStr.size()];
	int targetLength = targetStr.size();

	for (int i = 0; i < targetLength; ++i) {
		target[i] = toUnsignedChar(targetStr[i]);
	}

	int alphabetLength = 5;
	int k = -1;
	int mode = EDLIB_MODE_NW;
	bool findStartLocations = false;
	bool findAlignment = false;
	int score = 0;
	int* endLocations = nullptr; // dummy
	int* startLocations = nullptr; // dummy
	int numLocations = 0; // dummy;
	unsigned char* alignemnt = nullptr; // dummy
	int alignmentLength = 0; // dummy;

	edlibCalcEditDistance(query, queryLength, target, targetLength, alphabetLength,
		k, mode, findStartLocations, findAlignment, &score, &endLocations,
		&startLocations, &numLocations, &alignemnt, &alignmentLength);

	free(alignemnt);
	free(startLocations);
	free(endLocations);

	delete[] target;
	delete[] query;

	return score;
}

void Graph::deleteMarked() {
	deleteMarkedVertices();
	deleteMarkedEdges();
}

void Graph::deleteMarkedEdges() {
	set<int> dirty_vertices;
	vector<Edge*> removed_edges;

	int confirmed = 0;
	for (int i = 0, n = edges.size(); i < n; ++i) {
		Edge* edge = edges[i];
		if (edge->isMarked()) {
			dirty_vertices.insert(edge->getSrc()->getId());
			dirty_vertices.insert(edge->getDst()->getId());
			removed_edges.push_back(edge);
			continue;
		}

		edges[confirmed] = edges[i];
		confirmed++;
	}
	edges.resize(confirmed);

	for (int idx : dirty_vertices) {
		Vertex* vertex = getVertex(idx);
		if (vertex != nullptr) {
			getVertex(idx)->removeMarkedEdges();
		}
	}

	for (Edge* edge : removed_edges) {
		delete edge;
	}
}

void Graph::deleteMarkedVertices() {
	std::vector<int> for_removal;

	map<unsigned int, Vertex*>::iterator it;
	for (it = vertices.begin(); it != vertices.end(); it++) {
		Vertex* v = it->second;
		if (!v->isMarked()) {
			continue;
		}

		for (Edge* edge : v->getEdgesB()) {
			edge->mark();
			edge->pair()->mark();
		}
		for (Edge* edge : v->getEdgesE()) {
			edge->mark();
			edge->pair()->mark();
		}
		for_removal.push_back(v->getId());
	}

	for (int idx : for_removal) {
		it = vertices.find(idx);
		Vertex* v = it->second;
		vertices.erase(it);
		delete v;
	}
}
