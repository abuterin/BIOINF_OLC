#include "Assembler.hpp"

Assembler* Assembler::assembler = nullptr; //initialization of private static member 'assembler'

std::pair<int, int>* Assembler::calculateForcedHangs(unsigned int aLow, unsigned int aHigh, unsigned int aLen, unsigned int bLow, unsigned int bHigh, unsigned int bLen)
{
	std::pair<int, int>* hangs = new std::pair<int, int>();

	hangs->first = aLow - bLow;				//a_hang
	unsigned int bAfter = bLen - bHigh;		//number of bases B has after overlap
	unsigned int aAfter = aLen - aHigh;		//number of bases A has after overlap
	hangs->second = bAfter - aAfter;		//b_hang

	return hangs;
}
