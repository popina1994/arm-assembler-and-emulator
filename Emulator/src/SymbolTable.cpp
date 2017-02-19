#include "SymbolTable.h"
#include "EmulatorError.h"


SymbolTable::SymbolTable()
{
}


SymbolTable::~SymbolTable()
{
}

void SymbolTable::Insert(SymbolTableEntry * ste, int line)
{
	if (FindByName(ste->getName()) == NULL)
		st.push_back(ste);
	else
		throw LabelAlreadyDefined(line, " or section");
}

SymbolTableEntry* SymbolTable::FindByName(const string& name)
{
	for (int idx = 0; idx < st.size(); idx++)
		if (name == st[idx]->getName())
			return st[idx];
	return NULL;
}

vector<SymbolTableEntry*>& SymbolTable::GetVectorOfEntries()
{
	return st;
}
