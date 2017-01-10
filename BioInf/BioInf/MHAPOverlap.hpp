#pragma once

/**
	Made by  Ante
*/
class MHAPOverlap {
private:
	unsigned int _Aid;		//ID of read A
	unsigned int _Bid;		//ID of read B
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

	MHAPOverlap(MHAPOverlap& overlap) : _Aid{ overlap.Aid() }, _Bid{ overlap.Bid() }, _jaccardScore{ overlap.JaccardScore() }, 
		_sharedMinMers{ overlap.sharedMinMers() }, _Aforward{ overlap.Aforward() }, _Astart{ overlap.Astart() }, _Aend{ overlap.Aend() }, 
		_Alength{ overlap.Alength() }, _Bforward{ overlap.Bforward() }, _Bstart{ overlap.Bstart() }, _Bend{ overlap.Bend() }, 
		_Blength{ overlap.Blength() } {}

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

	double coveredPercentageReadA() { return ((double)(_Aend - _Astart)) / _Alength; }
	double coveredPercentageReadB() { return ((double)(_Bend - _Bstart)) / _Blength; }
};