#pragma once
#include<vector>
#include<cstdio>

using namespace std;

/**
	Class created by Ante
*/
class Read {
private:
	string _read;
	string _reverseComplement;
	unsigned int _ID;

public:
	Read(string read, unsigned int ID) : _read{ read }, _ID{ ID } {}

	string read() { return _read; }
	unsigned int ID() { return _ID; }

	string reverseComplement();
};