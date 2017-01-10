#pragma once

#include "MHAPOverlap.hpp"
#include "MHAPOverlap.hpp"

class DovetailOverlap : public MHAPOverlap{
private:
	int _aHang;
	int _bHang;

public:
	DovetailOverlap(int aHang, int bHang, MHAPOverlap& overlap) : _aHang{ aHang }, _bHang{ bHang },
		MHAPOverlap{ overlap } {}

	int aHang() { return _aHang; }
	int bHang() { return _bHang; }

};