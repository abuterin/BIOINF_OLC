#pragma once
#include<ostream>

/**
	Made by  Ante
*/
class MHAPOverlap {
private:
	unsigned int _aID;		//ID of read A
	unsigned int _bID;		//ID of read B
	double _jaccardScore;
	unsigned int _sharedMinMers;
	bool _aForward;	//true - A forward, false - A reversed complement
	unsigned int _aStart;
	unsigned int _aEnd;
	unsigned int _aLength;
	bool _bForward;	//true - B forward, false - B reversed complement
	unsigned int _bStart;
	unsigned int _bEnd;
	unsigned int _bLength;
	unsigned int _overlapLength;

	bool _positionA;			//as defined in (Rahle, 2014), page 15s -> true - begining, false - end
	bool _positionB;			//as defined in (Rahle, 2014), page 15s -> true - begining, false - end

public:
	MHAPOverlap(unsigned int Aid, unsigned int Bid, double jaccardScore, unsigned int sharedMinMers, bool Afwd,
		unsigned int Astart, unsigned int Aend, unsigned int Alen, bool Bfwd, unsigned int Bstart, unsigned int Bend,
		unsigned int Blen) ;

	MHAPOverlap(MHAPOverlap& overlap); 

	unsigned int aID() { return _aID; }
	unsigned int bID() { return _bID; }
	double jaccardScore() { return _jaccardScore; }
	unsigned int sharedMinMers() { return _sharedMinMers; }
	bool aForward() { return _aForward; }
	unsigned int aStart() { return _aStart; }
	unsigned int aEnd() { return _aEnd; }
	unsigned int aLength() { return _aLength; }
	bool bForward() { return _bForward; }
	unsigned int bStart() { return _bStart; }
	unsigned int bEnd() { return _bEnd; }
	unsigned int bLength() { return _bLength; }

	double coveredPercentageReadA() { return ((double)(_aEnd - _aStart)) / _aLength; }
	double coveredPercentageReadB() { return ((double)(_bEnd - _bStart)) / _bLength; }

	bool suffixA() { return _positionA; }  //true - begining, false - end
	bool suffixB() { return _positionB; }

	bool part(unsigned int ID) {
		if (ID == this->aID())
			return suffixA();
		else
			return suffixB();
	}

	bool isUsingSuffix(unsigned int);

	unsigned int overlapLength() { return _overlapLength; }

	unsigned int getLength(unsigned id) {
		if (id == this->aID())
			return aLength();
		else
			return bLength();
	}

	friend std::ostream& operator<<(std::ostream& output, MHAPOverlap o) {
		output << o._aID << " " << o._bID << " " << o._jaccardScore << " " << o._sharedMinMers << " " << (o._aForward ? 0 : 1) << " "
			<<o._aStart << " " << o._aEnd << " " << o._aLength << " " << (o._bForward ? 0 : 1) << " " << o._bStart << " " << o._bEnd
			<< " " << o._bLength;
		return output;
	}
};