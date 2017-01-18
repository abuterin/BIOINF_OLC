This is a project for class Bioinformatics on University of Zagreb - Faculty of Electrical Engineering and Computing
link for class page: http://www.fer.unizg.hr/predmet/bio

## Requirements (for running the software)
- g++ 
- GNU 

##Optional (IDE for easyest editing and debugging)
- Microsoft Visual Studio 


## Installation

To get the project on your computer, you must retrieve the project files. There are two ways to do it:

	1. clone repository
		git clone https://github.com/abuterin/BIOINF_OLC 
    2. download .zip file (from the same page mentioned above)
	
	
## Using the program

Open the BIOINF_OLC\BioInf folder, inside a file named "BioInf" (type Microsoft Visual Studio Solution).
An .sln file that opens a project inside Microsoft Visual Studio application.
Program can be built, debugged and run from there. 

On Linux:
Position yourself in /BIOINF_OLC/BioInf/BioInf/ file. Run next lines in command prompt:

	g++ -std=c++11 Read.cpp  Assembler.cpp Graph.cpp MHAPOverlap.cpp Edge.cpp edlib.cpp StringGraphComponent.cpp StringGraphWalk.cpp Utils.cpp Vertex.cpp Walk.cpp Main.cpp -o layout

	./layout lambda_reads.fasta out.mhap result.fasta

First line builds the program and creates .exe named overlap.
Second line runs the overlap program using two input files, one for reads and one for overlaps.



