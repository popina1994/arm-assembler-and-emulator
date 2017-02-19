#pragma once


#include "Tokenizer.h"
#include "SymbolTableEntry.h"
#include "SymbolTable.h"
#include "RellocationTable.h"

using namespace std;

class Assembler
{
	static const int CHAR_SIZE = 8;
	static const int WORD_SIZE = 16;
	static const int LONG_SIZE = 32;
	ifstream* inFile;
	ofstream* outFile;
	Tokenizer tokenizer;
	vector <SymbolTableEntry*> sections;
	SymbolTable symbolTable;
	RellocationTable rellocationTable;
	
	vector<string> vTokens;
	int lastLabel;
	int line;
	bool labelExists;
	string curLine;

	int sizeOfSection;
	int argNum;
	bool end;
	Wraper wraper;

	int cntNibble;
	vector <SymbolTableEntry*> labels;

	void processSectionFirst(Wraper& wraper);

	void processLabel();

	void UpdateLabels();

	void processEndFirst();

	void processSkipFirst();

	void processWordFirst();

	void processCharFirst();

	void processLongFirst();

	void processAlignFirst();

	void processFirstExtern();

	void processFirstPublic();


	void processSectionSecond(Wraper& wraper);

	void processEndSecond();

	void processSkipSecond();

	void processWordSecond();

	void processCharSecond();

	void processLongSecond();

	void processAlignSecond();

	void processExternSecond();

	void processPublicSecond();

	void processIntSecond();

	void processAddSubTestSecond();
	
	void processMullDivSecond();

	void processAndOrNotTestSecond();

	void processLdrStrSecond();

	

	void processCallSecond();

	void processInOutSecond();

	void processMovShrShlSecond();

	void processLdchLdclSecond();
	
	void processLdcSecond();

	void output(const string& s, bool sectionName = false);

public:

	// Input and output file.
	//
	Assembler(ifstream* inFile, ofstream* outFile);
	
	// Fill the Symbol table.
	//
	void FirstPass();

	// Fill the rellocation table. 
	//
	void SecondPass();

	void OutputSymbolTable();

	void OutputRellocationTable();

};

