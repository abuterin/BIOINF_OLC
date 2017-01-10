#pragma once


class MHAPOverlap {
private:
	unsigned int _Aid;
	unsigned int _Bid;
	double _jaccardScore;
	unsigned int _sharedMinMers;
	bool _Aforward;	//true - A forward, false - A reversed complement
	unsigned int _Astart;
	unsigned int _Aend;
	unsigned int _Alength;
	bool _Bforward;	//true - B forward, false - B reversed complement
	unsigned int _Bstart;
	unsigned int _Bend;
	unsigned int _Blength;

public:
	MHAPOverlap(unsigned int Aid, unsigned int Bid, double jaccardScore, unsigned int sharedMinMers, bool Afwd,
		unsigned int Astart, unsigned int Aend, unsigned int Alen, bool Bfwd, unsigned int Bstart, unsigned int Bend,
		unsigned int Blen) : _Aid{ Aid }, _Bid{ Bid }, _jaccardScore{ jaccardScore }, _sharedMinMers{ sharedMinMers },
		_Aforward{ Afwd }, _Astart{ Astart }, _Aend{ Aend }, _Alength{ Alen }, _Bforward{ Bfwd }, _Bstart{ Bstart },
		_Bend{ Bend }, _Blength{ Blen } {}

	unsigned int Aid() { return _Aid; }
	unsigned int Bid() { return _Bid; }
	double JaccardScore() { return _jaccardScore; }
	unsigned int sharedMinMers() { return _sharedMinMers; }
	bool Aforward() { return _Aforward; }
	unsigned int Astart() { return _Astart; }
	unsigned int Aend() { return _Aend; }
	unsigned int Alength() { return _Alength; }
	bool Bforward() { return _Bforward; }
	unsigned int Bstart() { return _Bstart; }
	unsigned int Bend() { return _Bend; }
	unsigned int Blength() { return _Blength; }
};