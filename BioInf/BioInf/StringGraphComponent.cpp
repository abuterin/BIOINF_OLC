#include "StringGraphComponent.hpp"
using namespace std;

// contig extraction params
size_t MAX_BRANCHES = 18;
size_t MAX_START_NODES = 100;
double LENGTH_THRESHOLD = 0.05;
double QUALITY_THRESHOLD = 1.0;

//#define ABS(x) ((x < 0) ? x * (-1) : x)
#define MAX(x,y) ((x > y) ? x : y) 

#pragma region pomocneFunkcije
/*************************************************************************************** /
* Title: RNA Assembler source code
* Author : rvaser (robert.vaser@gmail.com)
* Date : Apr 13, 2015
* Availability : https://github.com/mariokostelac/ra
*************************************************************************************** */
static int expandVertex(std::vector<Edge*>& dst, Vertex* start, int start_direction, uint32_t maxId, int max_branches) {

	debug("EXPAND %d\n", start->getId());

	int totalLength = start->getLength();
	Vertex* vertex = start;
	int curr_direction = start_direction;

	std::vector<bool> visitedVertices(maxId + 1, false);

	while (true) {

		visitedVertices[vertex->getId()] = true;

		auto& edges = curr_direction == 0 ? vertex->getEdgesB() : vertex->getEdgesE();

		Edge* best_edge = nullptr;
		if (edges.size() == 1) {

			if (!visitedVertices[edges.front()->getDestinationNode()]) {
				best_edge = edges.front();
			}

		}
		else if (edges.size() > 1) {

			int best_length = 0;
			double best_qual = 0;
			double qual_lo = 0;

			for (auto& edge : edges) {
				best_qual = max(best_qual, overlap_score(edge->getOverlap()));
			}

			qual_lo = best_qual * (1 - QUALITY_THRESHOLD);

			for (auto& edge : edges) {

				Vertex* next = edge->getDst();

				if (visitedVertices[next->getId()]) {
					continue;
				}

				double curr_qual = overlap_score(edge->getOverlap());
				if (curr_qual >= qual_lo) {
					int curr_length = longest_sequence_length(next, edge->getOverlap()->isInnie() ? (curr_direction ^ 1) :
						curr_direction, visitedVertices, max_branches) + vertex->getLength() + edge->labelLength();

					if (curr_length > best_length) {
						best_edge = edge;
						best_length = curr_length;
					}
				}
			}
		}

		if (best_edge == nullptr) {
			break;
		}

		dst.emplace_back(best_edge);
		vertex = best_edge->getDst();

		totalLength += best_edge->labelLength();

		if (best_edge->overlap->isInnie()) {
			curr_direction ^= 1;
		}
	}

	return totalLength;
}

static int longest_sequence_length(Vertex* from, int direction, std::vector<bool>& visited,
	int forks_left) {

	if (forks_left < 0) {
		debug("STOPEXPAND %d because hit max branches %d\n", from->getId(), MAX_BRANCHES);
		return 0;
	}

	if (visited[from->getId()]) {
		debug("STOPEXPAND %d because visited\n", from->getId());
		return 0;
	}

	visited[from->getId()] = true;

	auto& edges = direction == 0 ? from->getEdgesB() : from->getEdgesE();

	int res_length = 0;

	if (edges.size() == 1) {

		auto& edge = edges.front();
		res_length += edge->labelLength();
		res_length += longest_sequence_length(edge->getDst(), edge->getOverlap()->is_innie() ?
			(direction ^ 1) : direction, visited, forks_left);

	}
	else if (edges.size() > 1) {

		Edge* best_edge = nullptr;

		int best_len = 0;
		double best_qual = 0;
		double qual_lo = 0;

		for (auto& edge : edges) {
			best_qual = max(best_qual, overlap_score(edge->getOverlap()));
		}

		qual_lo = best_qual * (1 - QUALITY_THRESHOLD);

		for (auto& edge : edges) {

			auto curr_qual = overlap_score(edge->getOverlap());

			if (curr_qual >= qual_lo) {
				int curr_len = longest_sequence_length(edge->getDst(), edge->getOverlap()->is_innie() ? (direction ^ 1) :
					direction, visited, forks_left - 1);

				if (curr_len > best_len) {
					best_edge = edge;
					best_len = curr_len;
				}
			}
		}

		if (best_edge != nullptr) {
			res_length += best_edge->labelLength();
			res_length += best_len;
		}
	}

	visited[from->getId()] = false;

	return res_length;
}

double overlap_score(MHAPOverlap* overlap) {
	double quality = 1 - overlap->err_rate();
	return (overlap->covered_percentage(overlap->aID()) + overlap->covered_percentage(overlap->bID())) * quality;
};

static int findSingularChain(std::vector<Edge*>* dst, Vertex* start, int start_direction) {

	int totalLength = start->getLength();
	Vertex* curr_vertex = start;
	int curr_direction = start_direction;

	while (true) {

		auto& edges = curr_direction == 0 ? curr_vertex->getEdgesB() : curr_vertex->getEdgesE();

		if (edges.size() == 0) {
			// end of chain
			break;
		}

		if (curr_vertex->edges_b.size() + curr_vertex->edges_e.size() > 2) {
			// not singular chain anymore
			break;
		}

		Edge* selectedEdge = edges.front();

		if (dst != nullptr) dst->emplace_back(selectedEdge);
		curr_vertex = selectedEdge->getDst();

		totalLength += selectedEdge->labelLength();

		if (selectedEdge->getOverlap()->is_innie()) {
			curr_direction ^= 1;
		}
	}

	return totalLength;
}

static int countForks(Vertex* start, int start_direction, int depth) {

	if (depth <= 0) {
		return 0;
	}

	int forks = 0;
	Vertex* curr_vertex = start;
	int curr_direction = start_direction;

	auto& edges = curr_direction == 0 ? curr_vertex->getEdgesB() : curr_vertex->getEdgesE();

	if (curr_vertex->edges_b.size() + curr_vertex->edges_e.size() > 2) {
		forks++;
	}

	for (auto e : edges) {
		curr_vertex = e->getDst();
		if (e->getOverlap()->is_innie()) {
			curr_direction ^= 1;
		}

		forks += countForks(curr_vertex, curr_direction, depth - 1);
	}

	return forks;
}

#pragma endregion


StringGraphComponent::StringGraphComponent(set<int> vertexIds, Graph* graph) :
	vertices_(), graph_(graph) {

	for (auto& id : vertexIds) {
		vertices_.emplace_back(graph->getVertexById(id));
	}

	walk_ = nullptr;
}

StringGraphComponent::~StringGraphComponent() {
	delete walk_;
}

void StringGraphComponent::extractSequence(string& dst) {

	if (walk_ == nullptr) extractLongestWalk();
	if (walk_ != nullptr) walk_->extractSequence(dst);
}

StringGraphWalk* StringGraphComponent::longestWalk() {
	if (walk_ == nullptr) extractLongestWalk();
	return walk_;
}

void StringGraphComponent::extractLongestWalk() {

	typedef std::tuple<Vertex*, int, double> Candidate;

	// pick top N start vertices - criterion is the length of the 
	//contiguous path from a starting vertex to the first branching vertex.
	std::vector<Candidate> startCandidates;

	uint32_t maxId = 0;
	for (auto& vertex : vertices_) {
		maxId = max(maxId, vertex->readID);
	}

	// tips and singular chains could be good candidates
	for (int direction = 0; direction <= 1; ++direction) {

		for (auto& vertex : vertices_) {

			if ((direction == 0 && vertex->edges_b.size() == 1 && vertex->edges_e.size() == 0) ||
				(direction == 1 && vertex->edges_e.size() == 1 && vertex->edges_b.size() == 0)) {

				vector<bool> visited(maxId + 1, false);
				startCandidates.emplace_back(vertex, direction, longest_sequence_length(vertex, direction,
					visited, 0));
			}
		}
	}

	// forks could be good candidates, too
	for (int direction = 0; direction <= 1; ++direction) {

		for (auto& vertex : vertices_) {

			if ((direction == 0 && vertex->edges_b.size() > 1) ||
				(direction == 1 && vertex->edges_e.size() > 1)) {

				std::vector<bool> visited(maxId + 1, false);
				startCandidates.emplace_back(vertex, direction, longest_sequence_length(vertex, direction,
					visited, 1));
			}
		}
	}

	// circular component
	if (startCandidates.size() == 0) {
		std::vector<bool> visited(maxId + 1, false);

		int direction = 0;
		auto vertex = vertices_.front();

		startCandidates.emplace_back(vertex, direction, longest_sequence_length(vertex, direction,
			visited, 1));
	}

	sort(startCandidates.begin(), startCandidates.end(),
		[](Candidate left, Candidate right) {
		return std::get<2>(left) > std::get<2>(right);
	}
	);

	int n = min(MAX_START_NODES, startCandidates.size());

	// expand each of n candidates to a full chain and pick the best one (by length)
	StringGraphWalk* selectedContig = nullptr;
	int selectedLength = 0;

	//Timer timer;
	//timer.start();

#pragma omp parallel for default(none), shared(maxId, n, MAX_BRANCHES, startCandidates, selectedLength, selectedContig), schedule(dynamic, 1)
	for (int i = 0; i < n; ++i) {

		Vertex* start = std::get<0>(startCandidates[i]);
		int direction = std::get<1>(startCandidates[i]);

		debug("CREATECONTIG from vertex %d\n", start->getId());

		std::vector<Edge*> edges;
		int length = expandVertex(edges, start, direction, maxId, MAX_BRANCHES);

#pragma omp critical
		if (length > selectedLength) {

			selectedLength = length;

			if (selectedContig != nullptr) {
				delete selectedContig;
			}

			selectedContig = new StringGraphWalk(start);
			for (auto& edge : edges) {
				selectedContig->addEdge(edge);
			}
		}
	}

	//timer.stop();
	//timer.print("SG", "extract longest walk");

	walk_ = selectedContig;
}
