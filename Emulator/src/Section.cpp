#include "Section.h"


bool Section::operator==(const Section &s) const
{
	return (s.hasBegin == hasBegin) && (s.beginAddress == beginAddress) && (s.name == name);
}

bool Section::operator<(const Section & s) const
{
	if (hasBegin) {
		if (!s.hasBegin)
			return true;
		return beginAddress < s.beginAddress;
	}
	else {
		if (s.hasBegin)
			return false;
		return name < s.name;
	}
		
}

RellocationTable & Section::getRellocationTable()
{
	return rellocationTable;
}

SymbolTable & Section::getSymbolTable()
{
	return symbolTable;
}

string Section::getName()
{
	return name;
}

void Section::setBeginAddress(int _address)
{
	hasBegin = true;
	beginAddress = _address;
}

int Section::getBeginAddress()
{
	return beginAddress;
}

void Section::AddSectionContent(string  s)
{
	sectionContent.append(s);
}

void Section::setSymbolTableEntry(SymbolTableEntry * ste)
{
	thisSection = ste;
}

SymbolTableEntry * Section::getSymbolTableEntry()
{
	return thisSection;
}

void Section::setSize(int _size)
{
	size = _size;
}

int Section::getSize()
{
	return size;
}

bool Section::hasBeginAddress()
{
	return hasBegin;
}

void Section::ResolveSymbol(RellocationTableEntry* rte, int glVal, Tokenizer& tokenizer)
{
	string type = rte->getType();
	int offset = rte->getOffsetSection() * 2;
	// 8 nibbles.
	//
	string instr = sectionContent.substr(offset, 8);
	if ((type == "ldch") || (type == "ldcl")) {
		string immediate = instr.substr(4, 4);
		int val = tokenizer.ConvertImmediateNumber(immediate, true);
		val += glVal;
		string binary = tokenizer.convertDecimalToBinary(val, 16);
		string hex = tokenizer.convertBinaryToHex(binary, -1);
		sectionContent.replace(offset+4, 4, hex, 0, 4);
	}
	else if (type == ".long") {
		string immediate = instr.substr(0, 8);
		tokenizer.littleEndian(immediate);
		int val = tokenizer.ConvertImmediateNumber(immediate, true);
		val += glVal;
		string binary = tokenizer.convertDecimalToBinary(val, 32);
		string hex = tokenizer.convertBinaryToHex(binary, -1);
		tokenizer.littleEndian(hex);
		sectionContent.replace(offset, 8, hex, 0, 8);
	}
	else if (type == "ldclExt") {
		string immediate = instr.substr(4, 4);
		int valLow = tokenizer.ConvertImmediateNumber(immediate, true);
		// ldchExt.
		//
		instr = sectionContent.substr(offset +8, 8);
		immediate = instr.substr(4, 4);
		int valHigh = tokenizer.ConvertImmediateNumber(immediate, true);
		int val = (valHigh << 16) | valLow;
		val += glVal;

		valLow = val & ((1 << 16) - 1);
		valHigh = (val & (~((1 << 16) - 1))) >> 16;
		string binaryLow = tokenizer.convertDecimalToBinary(valLow, 16);
		string binaryHigh = tokenizer.convertDecimalToBinary(valHigh, 16);
		string hexHigh = tokenizer.convertBinaryToHex(binaryHigh, -1);
		string hexLow = tokenizer.convertBinaryToHex(binaryLow, -1);
		sectionContent.replace(offset + 4, 4, hexLow, 0, 4);
		sectionContent.replace(offset + 12, 4, hexHigh, 0, 4);
	}
}

string Section::getType()
{
	if (name[1] == 't')
		return ".text";
	else if (name[1] == 'd')
		return ".data";
	else
		return ".bss";
}

const string & Section::getContent()
{
	return sectionContent;
}


Section::~Section()
{
}
