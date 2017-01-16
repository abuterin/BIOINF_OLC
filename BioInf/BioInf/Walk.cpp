#include "Walk.hpp"

vector<Walk*> Walk::extend(bool direction, Graph* graph) {
	vector<Edge*> _edges;
	vector<Walk*> walks;
	map<unsigned int, Vertex*> vertices = graph->vertices;

	if (direction) {		//if true, egde uses reads start
		_edges = _lastNode->edges_b;
	}
	else {					//otherwise it uses reds end
		_edges = _lastNode->edges_e;
	}

	if (_edges.empty())
		return walks;	//empty vector

	if (_edges.size() == 1) {	//there is no forking
		if (_edges[0]->overlap->aID() == this->_lastNode->readID) {
			if (_edges[0]->isInWalk()) {
				return walks;		//already in walk
			}
			_nodes.push_back(vertices[_edges[0]->overlap->bID()]);	//add vertex from the opposite end of the edge
		}
		else {
			if (_edges[0]->isInWalk()) {
				return walks;		//already in walk
			}
			_nodes.push_back(vertices[_edges[0]->overlap->aID()]);	//add vertex from the opposite end of the edge
		}

		_edges[0]->setInWalk(true);
		graph->getEdgeById(_edges[0]->pairId)->setInWalk(true);//mark edge and its pair edge as part of the walk
		_lastNode = _nodes.back();
		walks.push_back(this);
	}
	else {
		for (size_t i = 0; i < _edges.size(); i++) {
			Walk* newWalk;		//new forked walk
			Edge* edge = _edges[i]; //current edge

			if (i == _edges.size() - 1) {	//save last forking in current (this) walk
				if (edge->overlap->aID() == this->_lastNode->readID) {
					if (edge->isInWalk()) {
						continue;		//already in walk
					}

					_nodes.push_back(vertices[edge->overlap->bID()]);	//add vertex from the opposite end of the edge
				}
				else {
					if (edge->isInWalk()) {
						continue;		//already in walk
					}

					_nodes.push_back(vertices[edge->overlap->aID()]);	//add vertex from the opposite end of the edge
				}

				_lastNode = _nodes.back();
				walks.push_back(this);
			}

			else {

				if (edge->overlap->aID() == this->_lastNode->readID) {
					if (edge->isInWalk()) {
						continue;		//already in walk
					}
					newWalk = new Walk(this);	//new forked walk
					newWalk->addNode(vertices[edge->overlap->bID()]);	//add vertex from the opposite end of the edge
				}
				else {
					if (edge->isInWalk()) {
						continue;		//already in walk
					}

					newWalk = new Walk(this);	//new forked walk
					newWalk->addNode(vertices[edge->overlap->aID()]);	//add vertex from the opposite end of the edge
				}

				walks.push_back(newWalk);
			}

			edge->setInWalk(true);
			graph->getEdgeById(edge->pairId)->setInWalk(true);//mark edge and its pair edge as part of the walk
		}
	}

	return walks;
}
