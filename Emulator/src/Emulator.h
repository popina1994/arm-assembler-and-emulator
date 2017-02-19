#pragma once

#include <map>
#include <string>
#include "Section.h"
#include "RellocationTableEntry.h"
#include "RellocationTable.h"
#include "SymbolTable.h"
#include "SymbolTableEntry.h"
#include "Tokenizer.h"
#include "Memory.h"
#include "CPU.h"
#include "IVT.h"

using namespace std;

class Emulator
{
	map <string, Section*> mSection;
	map <string, SymbolTableEntry*> mExtern;
	map <string, SymbolTableEntry*> mGlobal;
	map <string, SymbolTableEntry*> mLoadScript;
	string loaderName;
	vector <string> asOutputNames;
	vector <string> vTokens;
	Tokenizer tokenizer;
	string lastName;
	string last;
	Section* lastSection;

	vector <Section*> vSection;

	unsigned argNum;
	unsigned line;
	int curAddress;

	string curLine;

	Memory memory;

	CPU cpu;

	IVT ivt;
	
	bool endEmulate;


	void ReadObjectFile(const string& nameOfFile);

	void AddSectionSymbol(const string&);

	void AddRellocationTable(const string&);

	void UpdateSectionAddress();

	void AddRellocationEntry(const string&);

	void AddSymbolTableEntry(const string& nameOfFile);

	int ParseRightSide(const string & curLiine);

	int Align(int address, int div);
public:

	Emulator(char* _loaderName, char** argV, int argN) : loaderName(_loaderName), memory(tokenizer),
	cpu (&memory, &ivt, &tokenizer), ivt(&memory, &tokenizer), endEmulate(false){
		for (int idx = 0; idx < argN; idx++)
			asOutputNames.push_back(string(argV[idx]));
	}

	void loadFiles();
	void readLoadScript();
	void resolveExtern();
	void sortSections();
	void resolveRellocations();
	void loadInMemory();
	void initSystem();
	void run();


	

	~Emulator();
};

