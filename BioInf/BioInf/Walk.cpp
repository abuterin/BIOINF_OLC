#include "Walk.hpp"
#include "Graph.hpp"

void Walk::addNode(Vertex * newNode) {
	_nodes.push_back(newNode); 
	_lastNode = newNode;
}

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

		_pathEdges.push_back(_edges[0]);
		_edges[0]->setInWalk(true);
		_edges[0]->pair()->setInWalk(true);//mark edge and its pairId edge as part of the walk
		_lastNode = _nodes.back();
		walks.push_back(this);
	}
	else {
		for (size_t i = 0; i < _edges.size(); i++) {
			Walk* newWalk;		//new forked walk
			Edge* edge = _edges[i]; //current edge
			if (edge == nullptr) {
				continue;
			}

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
			_pathEdges.push_back(edge);
			edge->setInWalk(true);
			if (edge->pair() != nullptr) {
				edge->pair()->setInWalk(true);//mark edge and its pairId edge as part of the walk
			}
		}
	}

	return walks;
}

void Walk::rewindTo(unsigned int junctionID) {
	size_t i;

	for (i = _nodes.size() - 1; i > 0; i--) {
		if (_nodes[i]->readID == junctionID) {
			break;
		}
		else {	//remove edges from walk
			for (size_t j = _pathEdges.size() - 1; j >= 0; j--) {
				Edge* edge = _pathEdges[j];
				if (edge->overlap->aID() == _nodes[i]->readID || edge->overlap->bID() == _nodes[i]->readID) {//if the edge contains current node
					edge->setInWalk(false);
					edge->pair()->setInWalk(false);
					_pathEdges.erase(_pathEdges.begin() + j);		//delete this member from the list
					break;
				}
			}
		}
	}

	_lastNode = _nodes[i];
	_nodes.resize(i + 1);
	return;
}

double Walk::coverage(bool direction) {
	if (_coverage == 0) {
		/*vector<Edge*> _edges;

		if (direction) {		//if true, egde uses reads start
			_edges = _lastNode->edges_b;
		}
		else {					//otherwise it uses reds end
			_edges = _lastNode->edges_e;
		}*/
		

		for (size_t i = 0; i < _nodes.size(); i++) {
			_coverage += _nodes[i]->readString.length() * 0.5; //approximate
		}
	}
	
	return _coverage;
}

void Walk::extractSequence(string &dest) {
	if (_pathEdges.empty()) {
		dest = _nodes[0]->readString;
		return;
	}

	auto getType = [](Edge* edge, unsigned int id) -> int {
		if (edge->overlap->aID() == id) return 0;
		if (!edge->overlap->isInnie()) return 0;
		return 1;
	};

	int startType = getType(_pathEdges.front(), _nodes.front()->readID);

	bool appendToPrefix = (!_pathEdges.front()->overlap->isUsingSuffix(_nodes.front()->readID)) ^ startType; //isUssingPrefix

	string startSequence = string(startType ? _nodes.front()->getReverseComplement() : _nodes.front()->readString);

	dest = appendToPrefix ? string(startSequence.rbegin(), startSequence.rend()) : startSequence;

	int prevType = startType;

	for (Edge* e : _pathEdges) {
		int type = getType(e, e->sourceId);

		bool invert = type == prevType ? false : true;

		string label;
		if (invert) {
			e->rkLabel(label);
		}
		else {
			e->label(label);
		}

		dest += appendToPrefix ? string(label.rbegin(), label.rend()) : label;

		prevType = getType(e, e->destinationId) ^ invert;
	}

	if (appendToPrefix) {
		dest = string(dest.rbegin(), dest.rend());
	}
}
