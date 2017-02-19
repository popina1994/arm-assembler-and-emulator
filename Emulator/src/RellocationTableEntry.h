#pragma once

#include "SymbolTableEntry.h"

class RellocationTableEntry
{	


	// Offset in section which is going to be updated.
	//
	int offsetSection;
	
	
	// ldh
	// ldl
	// ldclExt
	// ldchExt
	// .long
	//
	string type;
	
	// Section which belongs to.
	//
	SymbolTableEntry* section;

	// Symbol which is going to be added.
	//
	SymbolTableEntry* entry;

public:
	RellocationTableEntry(string _type, int _offsetSection, SymbolTableEntry* _section, SymbolTableEntry* _entry) : section(_section), offsetSection(_offsetSection), type(_type),
		 entry(_entry)  {
	}

	int getOffsetSection();
	string getType();
	SymbolTableEntry* getSection();
	SymbolTableEntry* getEntry();
	~RellocationTableEntry();
};

