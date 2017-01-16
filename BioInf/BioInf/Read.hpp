#pragma once
#include "CommonHeaders.hpp"

using namespace std;

/**
	Class created by Ante
*/
class Read {
private:
	string _read;
	string _reverseComplement;
	unsigned int _ID;
	double _coverage;

public:
	Read(string read, unsigned int ID) : _read{ read }, _ID{ ID }, _coverage{ 1.0 } {}

	string read() { return _read; }
	unsigned int ID() { return _ID; }
	double coverage() { return _coverage; }

	string reverseComplement();
};