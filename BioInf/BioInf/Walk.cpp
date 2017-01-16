#include "Walk.hpp"

vector<Walk*> Walk::extend(bool direction) {
	vector<Edge*> _edges;
	vector<Walk*> walks;
	if (direction) {		//if true, egde uses reads start
		_edges = _lastNode->edges_b;
	}
	else {					//otherwise it uses reds end
		_edges = _lastNode->edges_e;
	}

	if (_edges.empty())
		return walks;	//empty vector

	if (_edges.size() == 1) {	//there is no forking
		_nodes.push_back(_edges[0]->); //ovdje sam stao:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::....
	}
	else {
		for (size_t i = 0; i < _edges.size(); i++) {
			Walk* newWalk;		//new forked walk
			if (i == _edges.size() - 1) {	//save last forking in current (this) walk
				if (_edges[i]->overlap->aID == this->_lastNode->readID) {

				}
			}
		}
	}
}
