#include "MHAPOverlap.hpp"

MHAPOverlap::MHAPOverlap(unsigned int Aid, unsigned int Bid, double jaccardScore, unsigned int sharedMinMers, bool Afwd, 
	unsigned int Astart, unsigned int Aend, unsigned int Alen, bool Bfwd, unsigned int Bstart, unsigned int Bend, 
	unsigned int Blen) : _aID{ Aid }, _bID{ Bid }, _jaccardScore{ jaccardScore }, _sharedMinMers{ sharedMinMers },
	_aForward{ Afwd }, _aStart{ Astart }, _aEnd{ Aend }, _aLength{ Alen }, _bForward{ Bfwd }, _bStart{ Bstart },
	_bEnd{ Bend }, _bLength{ Blen } {

	if (_aLength - _aEnd < _aStart) {		//if end of overlap is closer to end of sequence read, than overlap beginning
		_positionA = false;					//is to beginning of sequence read
	}
	else if (_aStart < _aLength - _aEnd) {
		_positionA = true;
	}

	if (_bLength - _bEnd < _bStart) {		//if end of overlap is closer to end of sequence read, than overlap beginning
		_positionB = false;					//is to beginning of sequence read
	}
	else if (_bStart < _bLength - _bEnd) {
		_positionB = true;
	}

	_overlapLength = _aEnd - _aStart;		//aproximate length
}

MHAPOverlap::MHAPOverlap(MHAPOverlap & overlap) : _aID{ overlap.aID() }, _bID{ overlap.bID() }, _jaccardScore{ overlap.jaccardScore() },
_sharedMinMers{ overlap.sharedMinMers() }, _aForward{ overlap.aForward() }, _aStart{ overlap.aStart() }, _aEnd{ overlap.aEnd() },
_aLength{ overlap.aLength() }, _bForward{ overlap.bForward() }, _bStart{ overlap.bStart() }, _bEnd{ overlap.bEnd() },
_bLength{ overlap.bLength() } {

	if (_aLength - _aEnd < _aStart) {		//if end of overlap is closer to end of sequence read, than overlap beginning
		_positionA = false;					//is to beginning of sequence read
	}
	else if (_aStart < _aLength - _aEnd) {
		_positionA = true;
	}

	if (_bLength - _bEnd < _bStart) {		//if end of overlap is closer to end of sequence read, than overlap beginning
		_positionB = false;					//is to beginning of sequence read
	}
	else if (_bStart < _bLength - _bEnd) {
		_positionB = true;
	}

	_overlapLength = _aEnd - _aStart;		//aproximate length
}



bool MHAPOverlap::isInnie() {
	return _aForward == !_bForward; //if A and B directions are opposite
}
