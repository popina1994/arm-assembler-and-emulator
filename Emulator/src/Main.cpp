#include <cstdio>
#include <iostream>
#include <fstream>
#define HAVE_STRUCT_TIMESPEC
#include <pthread.h>

#include "EmulatorError.h"
#include "Emulator.h"
#include "Memory.h"


const bool DEBUG = false;

using namespace std;



int main(int argC, char** argV) {

	Tokenizer t;
	Memory m(t);

	try {
		// NameOfFIle, input file, output file
		//
		if (argC <= 2) {
			throw WrongNumberOfArguments(-1);
		}

		if (DEBUG)
			freopen("std.out", "w", stdout);

		Emulator* emulator = new Emulator(argV[1], argV + 2, argC - 2);
		// Load sections, check duplicates, load entries.
		//
		emulator->loadFiles();
		// Read scipt, and update labels, section position.
		//
		emulator->readLoadScript();
		// It will cause an error if there are unresolved extern symbols.
		//
		emulator->resolveExtern();
		// Sort sections, as in load script.
		//
		emulator->sortSections();
		// Resolve relocations, and prepare for memory loading.
		//
		emulator->resolveRellocations();

		// Loads sections in memory.
		//
		emulator->loadInMemory();
		// Initializes system.
		//
		emulator->initSystem();
		// Runs emulator.
		//
		emulator->run();
	}
	// Polymorphism.
	//
	catch (EmulatorError& ae) {
		cout << ae.GetError();
		exit(ae.exitCodeReturn());
	}
	if (DEBUG)
		cout << "Sucessfull" << endl;

	return 0;

}
