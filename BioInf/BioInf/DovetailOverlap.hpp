#pragma once

#include "MHAPOverlap.hpp"

class DovetailOverlap {
private:
	int _aHang;
	int _bHang;
	unsigned int _Aid;		//ID of read A
	unsigned int _Bid;		//ID of read B

public:
	DovetailOverlap(int aHang, int bHang, unsigned int Aid, unsigned int Bid) : _aHang{ aHang }, _bHang{ bHang },
		_Aid{ Aid }, _Bid{ Bid } {}

	int aHang() { return _aHang; }
	int bHang() { return _bHang; }
	unsigned int Aid() { return _Aid; }
	unsigned int Bid() { return _Bid; }

};