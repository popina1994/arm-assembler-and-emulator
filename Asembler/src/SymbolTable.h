#pragma once

#include "SymbolTableEntry.h"
#include <vector>

using namespace std;

class SymbolTable
{
	vector <SymbolTableEntry*> st;
public:
	SymbolTable();
	~SymbolTable();

	void Insert(SymbolTableEntry* st, int line);
	SymbolTableEntry* FindByName(const string& name);
	vector <SymbolTableEntry*>& GetVectorOfEntries();
};

