#pragma once

#include "CommonHeaders.hpp"

//#include "MHAPOverlap.hpp"

/**
	Class created by Ante
*/
class DovetailOverlap : public MHAPOverlap{
private:
	int _aHang;
	int _bHang;

	uint32_t a_lo_;
	uint32_t a_hi_;
	uint32_t b_lo_;
	uint32_t b_hi_;

public:
	DovetailOverlap(int aHang, int bHang, MHAPOverlap& overlap) : MHAPOverlap( overlap ), _aHang { aHang }, _bHang{ bHang } {
		
		a_lo_ = _aHang > 0 ? _aHang : 0;
		a_hi_ = aLength() - (_bHang < 0 ? -_bHang : 0);
		b_lo_ = _aHang < 0 ? -_aHang : 0;
		b_hi_ = bLength() - (_bHang > 0 ? _bHang : 0);
	}

	int aHang() { return _aHang; }
	int bHang() { return _bHang; }

	unsigned int length(unsigned int read_id) const {
		if (read_id == _aID) {
			return a_hi_ - a_lo_;
		}
		return b_hi_ - b_lo_;
	}

	bool isUsingSuffix(unsigned int readID) {
		if (readID == _aID) {
			return !_positionA;			//opposite of _positionA value - true if it is suffx, false if it is prefix
		}
		else if (readID == _bID) {
			return !_positionB;			//opposite of _positionA value - true if it is suffx, false if it is prefix
		}
		return false;
	}

	bool isUsingPrefix(unsigned int readID) {
		if (readID == _aID) {
			return !_positionA;			//opposite of _positionA value - true if it is suffx, false if it is prefix
		}
		else if (readID == _bID) {
			return !_positionB;			//opposite of _positionA value - true if it is suffx, false if it is prefix
		}
		return false;
	}
	/*
	bool Overlap::is_using_prefix(uint32_t read_id) const {

		if (read_id == a()) {
			if (a_hang_ <= 0) return true;

		}
		else if (read_id == b()) {
			if (is_innie_ == false && a_hang_ >= 0) return true;
			if (is_innie_ == true && b_hang_ <= 0) return true;
		}

		return false;
	}

	bool Overlap::is_using_suffix(uint32_t read_id) const {

		if (read_id == a()) {
			if (b_hang_ >= 0) return true;

		}
		else if (read_id == b()) {
			if (is_innie_ == false && b_hang_ <= 0) return true;
			if (is_innie_ == true && a_hang_ >= 0) return true;
		}

		return false;
	}
	*/

};