#include "SymbolTableEntry.h"

int SymbolTableEntry::globalID = 0;

bool SymbolTableEntry::operator<(const SymbolTableEntry& e) const
{
	return name < e.name;
}

bool SymbolTableEntry::operator==(const SymbolTableEntry & e) const
{
	return name == e.name;
}


void SymbolTableEntry::setSection(SymbolTableEntry * _section)
{
	section = _section;
}

void SymbolTableEntry::setVisibility(const string & s)
{
	visibility = s;
}

void SymbolTableEntry::setSize(int _size)
{
	size = _size;
}

void SymbolTableEntry::setValue(int _val)
{
	value = _val;
}

SymbolTableEntry * SymbolTableEntry::getSection()
{
	return section;
}

int SymbolTableEntry::getSize() const
{
	return size;
}

int SymbolTableEntry::getValue() const
{
	return value;
}

string SymbolTableEntry::getName()
{
	return name;
}

string SymbolTableEntry::getType() const
{
	if (value == -1)
		return ".extern";
	else if (size > 0)
		return "section";
	else if (visibility == "local")
		return "local";
	else
		return "global";
}

string SymbolTableEntry::getTypeOfSection()
{
	return typeOfSection;
}

string SymbolTableEntry::getVisibility()
{
	return visibility;
}

int SymbolTableEntry::getIdx() const
{
	return id;
}

SymbolTableEntry::~SymbolTableEntry()
{
}

bool SymbolTableEntry::isSectionEntry() const
{
	return size != -1;
}
