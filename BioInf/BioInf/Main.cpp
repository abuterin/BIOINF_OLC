#pragma once

#include<cstdio>
#include<cstdlib>
#include<iostream>
#include<fstream>
#include<vector>
#include<map>
#include<string>

#include "MHAPOverlap.hpp"
#include "DovetailOverlap.hpp"
#include "Assembler.hpp"
#include "Read.hpp"
#include "Graph.h"

using namespace std;

typedef struct vector<MHAPOverlap*> Overlaps;
typedef struct map<unsigned int, Read*> Reads;

int main(int argc, char *argv[]) {

	if (argc != 3) {
		cerr << "Excpeted two argument - first name of the .fasta file and then name of the .mhap file!" << endl;
		exit(-1);
	}

	if (strstr(argv[1], ".fasta") == nullptr) {
		cerr << "Wrong file format - expected .mhap file!" << endl;
		exit(-1);
	}

	if (strstr(argv[2], ".mhap") == nullptr) {
		cerr << "Wrong file format - expected .mhap file!" << endl;
		exit(-1);
	}
	
	ifstream fastaFile(argv[1]);
	FILE* mhapFile;
	fopen_s(&mhapFile, argv[2], "r");
	/*ofstream outFile;
	outFile.open("results.mhap");
	*/
	if (!fastaFile.is_open()) {
		cerr << "Couldn't open " << argv[1] << endl;
		exit(-1);
	}
	if (mhapFile == nullptr) {
		cerr << "Couldn't open " << argv[2] << endl;
		exit(-1);
	}
	/*
	if (!outFile.is_open()) {
		cerr << "Couldn't open results.mhap" << endl;
		exit(-1);
	}
	*/
	string line;
	bool sequenceLine = false; //true if next line to be read is sequence line, false if it is ID line
	unsigned int readID;
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

	Reads reads;//map<unsigned int, Read*>
	while (getline(fastaFile, line)) {
		if (!sequenceLine) {	//it has read ID line
			line.erase(line.begin());
			readID = stoi(line, nullptr);
		}
		else {
			pair<unsigned int, Read*> read{ readID, new Read(line, readID) };
			reads.insert(read);
		}
		sequenceLine = !sequenceLine;
		
	}
	fastaFile.close();

	Overlaps overlaps; // vector<MHAPOverlap*>
	while (fscanf_s(mhapFile, "%u %u %lf %u %d %u %u %u %d %u %u %u\n", &aID, &bID, &error, &sharedMinMers, &aFwd, &aStart, &aEnd,
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


	//******************************
	Graph ourGraph(reads,overlaps);
	ourGraph.simplify();

	for (size_t i = 0; i < reads.size(); i++) {
		delete reads[i];
	}
	for (MHAPOverlap* temp : overlaps) {
		delete temp;
	}
	overlaps.clear();

	return 0;
}