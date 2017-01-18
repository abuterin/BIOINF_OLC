//#pragma once

#include "CommonHeaders.hpp"

#include "Read.hpp"
#include "MHAPOverlap.hpp"
#include "DovetailOverlap.hpp"
#include "Assembler.hpp"
#include "StringGraphWalk.hpp"
#include "Graph.hpp"

#define MIN_COVERAGE (0.15)
#define MAX_ERROR (0.35)

using namespace std;

typedef struct vector<MHAPOverlap*> Overlaps;
typedef struct vector<DovetailOverlap*> DovetailOverlaps;
typedef struct map<unsigned int, Read*> Reads;

void writeOut(ofstream& outputFile, vector<StringGraphWalk*> unitings) {		//add unitings as parameters

	for (size_t i = 0; i < unitings.size(); i++) {				//dok god ima unintinga
		string dst;
		unitings[i]->extractSequence(dst);
		outputFile << ">contig " << i + 1 << endl;
		outputFile << dst << endl;		//test output  (kada prode build)
	}
}

int main(int argc, char *argv[]) {

	if (argc != 4) {
		cerr << "Excpeted three arguments - first name of the .fasta file with reads, then name of the .mhap file and at last the name of the output .fasta file!" << endl;
		exit(-1);
	}

	if (strstr(argv[1], ".fasta") == nullptr) {
		cerr << "Wrong file format - expected .fasta file!" << endl;
		exit(-1);
	}

	if (strstr(argv[2], ".mhap") == nullptr) {
		cerr << "Wrong file format - expected .mhap file!" << endl;
		exit(-1);
	}

	if (strstr(argv[3], ".fasta") == nullptr) {
		cerr << "Wrong file format - expected .fasta file!" << endl;
		exit(-1);
	}
	
	ifstream fastaFile(argv[1]);
	FILE* mhapFile;
	ofstream outputFile(argv[3]);
	mhapFile = fopen(argv[2], "r");
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
	
	if (!outputFile.is_open()) {
		cerr << "Couldn't open " << argv[3] << endl;
		exit(-1);
	}
	
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
			Read* read = new Read(line, readID);
			//pair<unsigned int, Read*> read{ readID, new Read(line, readID, line) };
			reads[readID] = read;
		}
		sequenceLine = !sequenceLine;
		
	}
	fastaFile.close();

	Overlaps overlaps; // vector<MHAPOverlap*>
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

	DovetailOverlaps dovetailOverlaps;
	Assembler* assembler = Assembler::getAssembler();
	
	//convert MHAPOverlaps to DovetailOverlaps
	for (size_t i = 0; i < overlaps.size(); i++) {
		dovetailOverlaps.push_back(assembler->calculateForcedHangs(*overlaps[i]));
	}
	cout << "Turned MHAP overlaps to Dovetail overlaps" << endl;
	assembler->filterContained(dovetailOverlaps, reads);
	cout << "Filtered contained overlaps" << endl;
	assembler->filterTransitiveOverlaps(dovetailOverlaps);
	cout << "Filtered transitive overlaps" << endl;
	assembler->filterShortOverlaps(dovetailOverlaps, MIN_COVERAGE);	
	cout << "Filtered short overlaps" << endl;
	assembler->filterErroneousOverlaps(dovetailOverlaps, MAX_ERROR);
	cout << "Filtered erroneous overlaps" << endl;
	//******************************Creates new graph
	Graph* ourGraph = new Graph(reads, dovetailOverlaps);		
	ourGraph->simplify();

	vector<StringGraphWalk*>* unitings = new vector<StringGraphWalk*>();

	ourGraph->extractUnitigs(unitings);

	writeOut(outputFile, *unitings);

	for (size_t i = 0; i < reads.size(); i++) {
		delete reads[i];
	}
	for (MHAPOverlap* temp : overlaps) {
		delete temp;
	}
	overlaps.clear();

	outputFile.close();

	delete ourGraph;
	delete assembler;

	return 0;
}
