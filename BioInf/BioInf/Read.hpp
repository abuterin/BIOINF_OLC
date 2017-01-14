#pragma once
#include<vector>
#include<cstdio>

using namespace std;

class Read {
private:
	string _read;
	unsigned int _ID;

public:
	Read(string read, unsigned int ID) : _read{ read }, _ID{ ID } {}

	string read() { return _read; }
	unsigned int ID() { return _ID; }
};