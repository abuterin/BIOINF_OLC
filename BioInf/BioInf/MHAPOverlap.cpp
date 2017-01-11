#include "MHAPOverlap.hpp"

MHAPOverlap::MHAPOverlap(unsigned int Aid, unsigned int Bid, double jaccardScore, unsigned int sharedMinMers, bool Afwd, 
	unsigned int Astart, unsigned int Aend, unsigned int Alen, bool Bfwd, unsigned int Bstart, unsigned int Bend, 
	unsigned int Blen) : _Aid{ Aid }, _Bid{ Bid }, _jaccardScore{ jaccardScore }, _sharedMinMers{ sharedMinMers },
	_Aforward{ Afwd }, _Astart{ Astart }, _Aend{ Aend }, _Alength{ Alen }, _Bforward{ Bfwd }, _Bstart{ Bstart },
	_Bend{ Bend }, _Blength{ Blen } {
	if (_Aend == _Alength) {
		_suffixA = false;
	}
	else if (_Astart == 0) {
		_suffixA = true;
	}

	if (_Bend == _Blength) {
		_suffixB = false;
	}
	else if (_Bstart == 0) {
		_suffixB = true;
	}

	_overlapLength = _Aend - _Astart;
}

MHAPOverlap::MHAPOverlap(MHAPOverlap & overlap) : _Aid{ overlap.Aid() }, _Bid{ overlap.Bid() }, _jaccardScore{ overlap.JaccardScore() },
_sharedMinMers{ overlap.sharedMinMers() }, _Aforward{ overlap.Aforward() }, _Astart{ overlap.Astart() }, _Aend{ overlap.Aend() },
_Alength{ overlap.Alength() }, _Bforward{ overlap.Bforward() }, _Bstart{ overlap.Bstart() }, _Bend{ overlap.Bend() },
_Blength{ overlap.Blength() } {
	if (_Aend == _Alength) {
		_suffixA = false;
	}
	else if (_Astart == 0) {
		_suffixA = true;
	}

	if (_Bend == _Blength) {
		_suffixB = false;
	}
	else if (_Bstart == 0) {
		_suffixB = true;
	}

	_overlapLength = _Aend - _Astart;
}
