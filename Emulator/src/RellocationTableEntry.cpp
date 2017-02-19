#include "RellocationTableEntry.h"




int RellocationTableEntry::getOffsetSection()
{
	return offsetSection;
}
string RellocationTableEntry::getType()
{
	return type;
}

SymbolTableEntry * RellocationTableEntry::getSection()
{
	return section;
}

SymbolTableEntry * RellocationTableEntry::getEntry()
{
	return entry;
}

RellocationTableEntry::~RellocationTableEntry()
{
}
