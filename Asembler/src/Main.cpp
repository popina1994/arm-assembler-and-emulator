#include "Assembler.h"
#include "AssemblingError.h"
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>

using namespace std;

int main(int argC, char** argV) {
	try {
		// NameOfFIle, input file, output file
		//
		if (argC != 3) {
			throw WrongNumberOfArguments(-1);
		}

		ifstream inFile(argV[1]);
		ofstream outFile(argV[2]);

		if (!inFile.is_open())
			throw InputFileProblem(-1);
		if (!outFile.is_open())
			throw OutputFileProblem(-1);
		freopen("std.out", "w", stdout);
		Assembler * assembler = new Assembler(&inFile, &outFile);

		assembler->FirstPass();
		assembler->SecondPass();
		assembler->OutputSymbolTable();
		assembler->OutputRellocationTable();
	}
	// Polymorphism.
	//
	catch (AssemblingError& ae) {
		cout << ae.GetNameOfError();
		exit(ae.exitCodeReturn());
	}
	return 0;
}

