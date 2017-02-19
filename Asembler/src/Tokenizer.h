#pragma once

#include <string>
#include <vector>
#include <set>

using namespace std;

class Wraper {
	// Operational code of instruction.
	//
	string opCode;
	// Type of instruction (directive, register...).
	//
	string type;
	// Name. (.extern, addal...)
	//
	string name;
	// Name of instruction (add, sub).
	//
	string instructionName;


	bool changeS;
public:
	Wraper(string _type, string _name, string _opCode, bool _changeS = false, const string&_instructionName="") : opCode(_opCode), 
		type(_type), name(_name), changeS(_changeS), instructionName(_instructionName){}
	Wraper(): opCode("BadCode") {}
	string getType();
	string getOpCode();
	string getName();
	bool getChangeS();

	void setType(const string&);
	void setOpCode(const string&);
	void setName(const string&);
	void setChangeS(bool);

	string getInstructionName();

	bool operator<(const Wraper& w) const {
		return name < w.name;
	}

	bool operator==(const Wraper& w) const {
		return name == w.name;
	}
		
};

class Tokenizer
{
	vector <Wraper> INSTRUCTION;
	vector <Wraper> CONDITION;
	vector <Wraper> FLAG;

	set <Wraper> KEY_WORD;
	set <string> DELIMITERS;
public:
	// Initializes all necessary values.
	//
	Tokenizer();

	string convertBinaryToHex(const string& s, int line);
	

	string convertDecimalToBinary(int val, int numBits);

	void littleEndian(string& binary);

	// Splits line of input to array of args.
	//
	void SplitString(const string & arg, vector<string>& vTokens, int line, bool& labelExists);

	bool IsKeyWord(const string&) const;

	// Checks is reg (PC,...)
	//
	bool isRegister(const string& arg, Wraper&);

	// check is immediate (hex, octo, dec)
	//
	bool isImmediate(const string& arg);

	// Is other
	//
	bool isOther(const string& arg, Wraper& w);
	
	// Check is operation.
	//
	bool isOpcode(const string& arg, Wraper&);

	bool isSection(const string & arg, Wraper & w, int line);
	
	int ConvertImmediateNumber(const string& arg);
	// Check is directive.
	//
	bool isDirective(const string& arg, Wraper&);

	bool isChar(const string& arg);

	int ConvertChar(const string& arg);

	~Tokenizer();
};

