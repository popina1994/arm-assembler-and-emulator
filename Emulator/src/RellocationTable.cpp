#include "RellocationTable.h"



void RellocationTable::Insert(RellocationTableEntry* rt)
{
	vRellocation.push_back(rt);
}

vector<RellocationTableEntry*>& RellocationTable::getRellocationVector()
{
	return vRellocation;
}

RellocationTable::RellocationTable()
{
}


RellocationTable::~RellocationTable()
{
}
