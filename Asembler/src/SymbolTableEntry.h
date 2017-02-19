#pragma once

#include <string>


using namespace std;

class SymbolTableEntry
{

	static int globalID;

	int id;
	
	// Name of label.
	//
	string name;
	
	// local, set in first pass.
	//
	string visibility;

	// Value of symbol.
	// 
	int value;

	// If this is not section this is -1 for local labels, for global 
	//
	int size;

	string typeOfSection;

	// When the linking is going to be done, this field is going to be used.
	//
	SymbolTableEntry* section;


public:

	bool operator<(const SymbolTableEntry&) const;

	bool operator==(const SymbolTableEntry&) const;
	SymbolTableEntry(string _name, string _visibility, int _value, string _typeOfSection ="not_sec", int _size = -1) : name(_name), visibility(_visibility), value(_value),
		size(_size), typeOfSection(_typeOfSection), id(globalID++) {}
	
	void setSection(SymbolTableEntry* section);
	void setVisibility(const string& s);
	// Only for sections.
	//
	void setSize(int size);
	void setValue(int _val);

	SymbolTableEntry* getSection();
	int getSize() const;
	int getValue() const;
	string getName();
	string getType() const;
	string getTypeOfSection();
	string getVisibility();
	int getIdx() const;



	~SymbolTableEntry();

	bool isSectionEntry() const;
};

