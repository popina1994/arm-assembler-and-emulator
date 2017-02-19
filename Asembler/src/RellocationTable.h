#pragma once

#include "RellocationTableEntry.h"
#include <vector>

using namespace std;

class RellocationTable
{
	vector <RellocationTableEntry*> vRellocation;
public:
	void Insert(RellocationTableEntry* rt);

	vector<RellocationTableEntry*>& getRellocationVector();

	RellocationTable();
	~RellocationTable();
};

