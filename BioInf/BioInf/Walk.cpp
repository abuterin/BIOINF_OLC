#include "Walk.hpp"

vector<Walk*> Walk::extend(bool direction) {
	vector<Edge*> _edges;
	if (direction) {//direction == B
		_edges = _lastNode->edges_b;
	}
	else
	{
		_edges = _lastNode->edges_e;
	}

	if (_edges.empty())
		return vector<Walk*>();	//empty vector

	if (_edges.size() == 1) {	//there is no forking
		//_nodes.push_back(_edges[0]->); ovdje sam stao:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::....
	}
}
