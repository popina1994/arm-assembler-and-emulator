#pragma once
#include <string>
#include "SymbolTable.h"
#include "SymbolTableEntry.h"
#include "RellocationTable.h"
#include "RellocationTableEntry.h"
#include "Tokenizer.h"

using namespace std;

class Section
{
	string 	name;
	SymbolTable symbolTable;
	RellocationTable rellocationTable;
	string sectionContent;
	bool hasBegin;
	int beginAddress;
	int size;
	SymbolTableEntry* thisSection;



public:
	bool operator==(const Section&) const;
	bool operator<(const Section&) const;
	Section(const string& _name) : name(_name), hasBegin(false), sectionContent() {};

	RellocationTable& getRellocationTable();
	SymbolTable& getSymbolTable();
	string getName();
	
	void setBeginAddress(int _address);
	int getBeginAddress();
	void AddSectionContent(string s);
	void setSymbolTableEntry(SymbolTableEntry* ste);
	SymbolTableEntry* getSymbolTableEntry();
	void setSize(int size);
	int getSize();
	bool hasBeginAddress();

	void ResolveSymbol(RellocationTableEntry* rte, int val, Tokenizer& tokenizer);

	string getType();

	const string& getContent();

	~Section();

};

