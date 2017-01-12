#pragma once

#include<cstdio>
#include<cstdlib>
#include<iostream>
#include<fstream>
#include<vector>
#include<unordered_set>
#include<string>

#include "MHAPOverlap.hpp"
#include "DovetailOverlap.hpp"
#include "Assembler.hpp"

using namespace std;

typedef struct vector<MHAPOverlap*> Overlaps;
typedef struct vector<string> Reads;

int main(int argc, char *argv[]) {

	if (argc != 2) {
		cerr << "Excpeted one argument - name of the .mhap file!" << endl;
		exit(-1);
	}

	if (strstr(argv[1], ".mhap") == nullptr) {
		cerr << "Wrong file format - expected .mhap file!" << endl;
		exit(-1);
	}
	
	FILE* mhapFile = fopen(argv[1], "r");
	/*ofstream outFile;
	outFile.open("results.mhap");
	*/
	if (mhapFile == nullptr) {
		cerr << "Couldn't open " << argv[1] << endl;
		exit(-1);
	}
	/*
	if (!outFile.is_open()) {
		cerr << "Couldn't open results.mhap" << endl;
		exit(-1);
	}
	*/
	unsigned int aID;
	unsigned int bID;
	double error;
	unsigned int sharedMinMers;
	int aFwd;
	int bFwd;
	unsigned int aStart;
	unsigned int bStart;
	unsigned int aEnd;
	unsigned int bEnd;
	unsigned int aLength;
	unsigned int bLength;
	/*size_t index = 0;*/

	Overlaps overlaps;
	while (fscanf(mhapFile, "%u %u %lf %u %d %u %u %u %d %u %u %u\n", &aID, &bID, &error, &sharedMinMers, &aFwd, &aStart, &aEnd,
		&aLength, &bFwd, &bStart, &bEnd, &bLength) == 12) {
		bool aForward = (aFwd == 1) ? false : true;
		bool bForward = (bFwd == 1) ? false : true;
		overlaps.push_back(new MHAPOverlap(aID, bID, error, sharedMinMers, aForward, aStart, aEnd, aLength, bForward, bStart,
			bEnd, bLength));
		
		/*outFile << *(overlaps[index]) << endl;
		index++;*/
	}
	fclose(mhapFile);
	/*outFile.close();*/

	for (MHAPOverlap* temp : overlaps) {
		delete temp;
	}
	overlaps.clear();

	return 0;
}