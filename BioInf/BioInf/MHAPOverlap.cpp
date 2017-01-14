#include "MHAPOverlap.hpp"

MHAPOverlap::MHAPOverlap(unsigned int Aid, unsigned int Bid, double jaccardScore, unsigned int sharedMinMers, bool Afwd, 
	unsigned int Astart, unsigned int Aend, unsigned int Alen, bool Bfwd, unsigned int Bstart, unsigned int Bend, 
	unsigned int Blen) : _aID{ Aid }, _bID{ Bid }, _jaccardScore{ jaccardScore }, _sharedMinMers{ sharedMinMers },
	_aForward{ Afwd }, _aStart{ Astart }, _aEnd{ Aend }, _aLength{ Alen }, _bForward{ Bfwd }, _bStart{ Bstart },
	_bEnd{ Bend }, _bLength{ Blen } {

	if (_aEnd == _aLength) {
		_suffixA = false;
	}
	else if (_aStart == 0) {
		_suffixA = true;
	}

	if (_bEnd == _bLength) {
		_suffixB = false;
	}
	else if (_bStart == 0) {
		_suffixB = true;
	}

	_overlapLength = _aEnd - _aStart;
}

MHAPOverlap::MHAPOverlap(MHAPOverlap & overlap) : _aID{ overlap.aID() }, _bID{ overlap.bID() }, _jaccardScore{ overlap.jaccardScore() },
_sharedMinMers{ overlap.sharedMinMers() }, _aForward{ overlap.aForward() }, _aStart{ overlap.aStart() }, _aEnd{ overlap.aEnd() },
_aLength{ overlap.aLength() }, _bForward{ overlap.bForward() }, _bStart{ overlap.bStart() }, _bEnd{ overlap.bEnd() },
_bLength{ overlap.bLength() } {
	if (_aEnd == _aLength) {
		_suffixA = false;
	}
	else if (_aStart == 0) {
		_suffixA = true;
	}

	if (_bEnd == _bLength) {
		_suffixB = false;
	}
	else if (_bStart == 0) {
		_suffixB = true;
	}

	_overlapLength = _aEnd - _aStart;
}
