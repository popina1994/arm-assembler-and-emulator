#include "Tokenizer.h"
#include <iostream>
#include "EmulatorError.h"

using namespace std;


Tokenizer::Tokenizer()
{
	/*
	// KEY_WORD
	//
	INSTRUCTION.push_back(Wraper("instruction", "int", "0000"));

	INSTRUCTION.push_back(Wraper("instruction", "add", "0001", true));
	INSTRUCTION.push_back(Wraper("instruction", "sub", "0010", true));
	INSTRUCTION.push_back(Wraper("instruction", "mul", "0011", true));
	INSTRUCTION.push_back(Wraper("instruction", "div", "0100", true));
	INSTRUCTION.push_back(Wraper("instruction", "cmp", "0101", true));
	
	INSTRUCTION.push_back(Wraper("instruction", "and", "0110", true));
	INSTRUCTION.push_back(Wraper("instruction", "or", "0111", true));
	INSTRUCTION.push_back(Wraper("instruction", "not", "1000", true));
	INSTRUCTION.push_back(Wraper("instruction", "test", "1001", true));

	INSTRUCTION.push_back(Wraper("instruction", "ldr", "1010"));
	INSTRUCTION.push_back(Wraper("instruction", "str", "1010"));
	INSTRUCTION.push_back(Wraper("instruction", "call", "1100"));

	INSTRUCTION.push_back(Wraper("instruction", "in", "1101"));
	INSTRUCTION.push_back(Wraper("instruction", "out", "1101"));
	
	INSTRUCTION.push_back(Wraper("instruction", "mov", "1110", true));
	INSTRUCTION.push_back(Wraper("instruction", "shr", "1110", true));
	INSTRUCTION.push_back(Wraper("instruction", "shl", "1110", true));

	INSTRUCTION.push_back(Wraper("instruction", "ldch", "1111"));
	INSTRUCTION.push_back(Wraper("instruction", "ldcl", "1111"));
	INSTRUCTION.push_back(Wraper("instruction", "ldc", "1111"));

	CONDITION.push_back(Wraper("condition", "eq", "000"));
	CONDITION.push_back(Wraper("condition", "ne", "001"));
	CONDITION.push_back(Wraper("condition", "gt", "010"));
	CONDITION.push_back(Wraper("condition", "ge", "011"));
	CONDITION.push_back(Wraper("condition", "lt", "100"));
	CONDITION.push_back(Wraper("condition", "le", "101"));
	CONDITION.push_back(Wraper("condition", "al", "111"));

	FLAG.push_back(Wraper("flag", "", "0"));
	FLAG.push_back(Wraper("flag", "s", "1"));


	for (int i = 0; i < INSTRUCTION.size(); i++)
		for (int j = 0; j < CONDITION.size(); j++) {
			KEY_WORD.insert(Wraper("instruction", INSTRUCTION[i].getName() + CONDITION[j].getName() + FLAG[0].getName(),
				CONDITION[j].getOpCode() + FLAG[0].getOpCode() + INSTRUCTION[i].getOpCode(), false,
				
				INSTRUCTION[i].getName()));

			if (INSTRUCTION[i].getChangeS()) {
				KEY_WORD.insert(Wraper("instruction", INSTRUCTION[i].getName() + CONDITION[j].getName() + FLAG[1].getName(),
					CONDITION[j].getOpCode() + FLAG[1].getOpCode() + INSTRUCTION[i].getOpCode(),
					true, INSTRUCTION[i].getName()));
			}
		}



	KEY_WORD.insert(Wraper("section", ".text", "-1"));
	KEY_WORD.insert(Wraper("section", ".data", "-1"));
	KEY_WORD.insert(Wraper("section", ".bss", "-1"));

	KEY_WORD.insert(Wraper("directive", ".public", "-2"));
	KEY_WORD.insert(Wraper("directive", ".extern", "-2"));
	KEY_WORD.insert(Wraper("directive", ".char", "-2"));
	KEY_WORD.insert(Wraper("directive", ".word", "-2"));
	KEY_WORD.insert(Wraper("directive", ".long", "-2"));
	KEY_WORD.insert(Wraper("directive", ".align", "-2"));
	KEY_WORD.insert(Wraper("directive", ".skip", "-2"));
	KEY_WORD.insert(Wraper("directive", ".end", "-2"));


	// WARNING !!!!!!!!!!!!!!!!!!!!!!!!!
	//
	KEY_WORD.insert(Wraper("reg", "r0", "0000"));
	KEY_WORD.insert(Wraper("reg", "r1", "0001"));
	KEY_WORD.insert(Wraper("reg", "r2", "0010"));
	KEY_WORD.insert(Wraper("reg", "r3", "0011"));
	KEY_WORD.insert(Wraper("reg", "r4", "0100"));
	KEY_WORD.insert(Wraper("reg", "r5", "0101"));
	KEY_WORD.insert(Wraper("reg", "r6", "0110"));
	KEY_WORD.insert(Wraper("reg", "r7", "0111"));
	KEY_WORD.insert(Wraper("reg", "r8", "1000"));
	KEY_WORD.insert(Wraper("reg", "r9", "1001"));
	KEY_WORD.insert(Wraper("reg", "r10", "1010"));
	KEY_WORD.insert(Wraper("reg", "r11", "1011"));
	KEY_WORD.insert(Wraper("reg", "r12", "1100"));
	KEY_WORD.insert(Wraper("reg", "r13", "1101"));
	KEY_WORD.insert(Wraper("reg", "r14", "1110"));
	KEY_WORD.insert(Wraper("reg", "r15", "1111"));
	KEY_WORD.insert(Wraper("specialreg", "pc", "10000"));
	KEY_WORD.insert(Wraper("specialreg", "lr", "10001"));
	KEY_WORD.insert(Wraper("specialreg", "sp", "10010"));
	KEY_WORD.insert(Wraper("specialreg", "psw", "10011"));

	KEY_WORD.insert(Wraper("other", "prefixInc", "100"));
	KEY_WORD.insert(Wraper("other", "postfixInc", "010"));
	KEY_WORD.insert(Wraper("other", "prefixDec", "101"));
	KEY_WORD.insert(Wraper("other", "postfixDec", "011"));

	DELIMITERS.insert(",");
	DELIMITERS.insert(" ");
	DELIMITERS.insert("\t");
	DELIMITERS.insert(":");
	*/
	DELIMITERS.insert(" ");
	DELIMITERS.insert("\t");
	DELIMITERS.insert("+");
	DELIMITERS.insert("-");
	DELIMITERS.insert("(");
	DELIMITERS.insert(")");
	DELIMITERS.insert(",");
	DELIMITERS.insert("=");
}



string Tokenizer::convertDecimalToBinary(int num, int numBits) {
	string ret = string( numBits, '0');
	int idxStr = ret.length() - 1;

	while (numBits > 0) {
		unsigned n = num & 0x1;
		
		ret[idxStr] = '0' + n;
		idxStr--;
		numBits--;
		num >>= 1;
	}
	return ret;
}

void Tokenizer::littleEndian(string & binary)
{
	int len = binary.length();
	static int number[] = { 2, 6 };
	for (int idx = 0; idx < len / 4; idx++)
		for (int idxIn = 0; idxIn < 2; idxIn++){
			int tmp = binary[idx * 2 + idxIn];
			if (len == 8) {
				binary[idx * 2 + idxIn] = binary[6 - idx * 2 + idxIn];
				binary[6 - idx * 2 + idxIn] = tmp;
			}
			else {
				binary[idx * 2 + idxIn] = binary[2 - idx * 2 + idxIn];
				binary[2 - idx * 2 + idxIn] = tmp;
			}
			
		}
}

// Just use it when output program data.
//
string Tokenizer::convertBinaryToHex(const string & s, int line)
{
	string ret = "";
	char letters[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

	for (int idx = 0; idx < s.length(); ) {
		unsigned curNum = 0;
		for (int cnt = 0; cnt < 4; cnt++, idx++) {
			
			if (s[idx] != '0' && s[idx] != '1') {
				throw NotBinaryNumber(line);
			}
			curNum = curNum * 2 + s[idx] - '0';

		}
		ret.append(string(1, letters[curNum]));
	}
	return ret;
}


// First pass need to check if symbols are in right order.
//
void Tokenizer::SplitString(const string & arg, vector<string>& vTokens, int line, const string& loaderName)
{
	string curStr = "";

	// If last token in line is different from , and :
	//
	bool lastTokenValid = true;
	// Has current string be pushed in vector.
	//
	bool pushed = false;
	bool lastRight = false;
	unsigned idx = 0;

	for (; idx < arg.size(); idx++) {
		if (DELIMITERS.find(string(1, arg[idx])) == DELIMITERS.end()) {			
			pushed = false;
			curStr.append(1, arg[idx]);
			lastTokenValid = true;
			lastRight = false;
		}
		else {
			if ((arg[idx] == '=')) {
				if ((curStr != "") && vTokens.size() == 0)
				{
					if (!pushed) { vTokens.push_back(curStr); pushed = true; curStr = ""; }
					vTokens.push_back(string(1, arg[idx]));
					lastTokenValid = false;
				}
				else throw WrongInstructionFormat(line, " assingment " + loaderName);
			}
			else if ((arg[idx] == '+') || (arg[idx] == '-') || (arg[idx] == '(') || (arg[idx] == ')') || (arg[idx] == ',')) {
				if ((vTokens.size() > 1) && (curStr != "") ) {

					if (!pushed) { vTokens.push_back(curStr); pushed = true; curStr = ""; }
					
					if (arg[idx] == ')' && lastRight)
						throw WrongInstructionFormat(line, " num )");
					if ((arg[idx] == ')') && (!lastRight)){
						pushed = false;
						curStr = string(1, arg[idx]);
						lastTokenValid = true;
						lastRight = true;
					}
					else {
						vTokens.push_back(string(1, arg[idx]));
						lastTokenValid = false;
						lastRight = false;
					}
				}
				else
					throw WrongInstructionFormat(line, " + ,-, *, (, )" + loaderName);
			}
		}
	}
	
	if (!lastTokenValid)
		throw WrongInstructionFormat(line, "last token" +loaderName);
	if (!pushed && curStr != "")
		vTokens.push_back(curStr);
	
	if (vTokens.size() < 3) 
		throw WrongInstructionFormat(line, "Number of operands");
		
}





bool Tokenizer::IsKeyWord(const string& s) const
{
	return KEY_WORD.find(Wraper("", s, "")) != KEY_WORD.end();
}

void ToLower(string& s) {
	for (int idx = 0; idx < s.length(); idx++)
		if (s[idx] >= 'A' && s[idx] <= 'Z')
			s[idx] = s[idx] - 'A' + 'a';
}

bool Tokenizer::isRegister(const string & arg, Wraper & w)
{	
	w.setName(arg);
	set<Wraper>::iterator it = KEY_WORD.find(w);
	if (it == KEY_WORD.end())
		return false;

	w = *it;
	if ((w.getType() == "reg") || (w.getType() == "specialreg")) {
		return true;
	}
	return false;
}

bool Tokenizer::isImmediate(const string & arg)
{
	int idx = 0;
	string copyArg = arg;
	ToLower(copyArg);

	if ((arg.length() >= 2) && (arg[0] == '0') && (arg[1] == 'x')) {
		for (idx = 2; idx < copyArg.length(); idx++) {
			if (!((copyArg[idx] >= '0' && (copyArg[idx] <= '9')) ||
				((copyArg[idx] >= 'a') && (copyArg[idx] <= 'f'))))
				return false;
		}
	}
	else if (arg[idx] == '0') {
		for (idx = 1; idx < copyArg.length(); idx++) {
			if (copyArg[idx] < '0' || copyArg[idx] >'7')
				return false;
		}
	}
	else {
		if (copyArg[0] == '+' || copyArg[0] == '-')
			idx = 1;
		for (;idx < copyArg.length(); idx++)
			if (!(copyArg[idx] <= '9' && copyArg[idx] >= '0'))
				return false;
	}
	return true;
}

bool Tokenizer::isOther(const string & arg, Wraper& w)
{

	set<Wraper>::iterator it;
	w.setName(arg);

	it = KEY_WORD.find(w);

	if (it == KEY_WORD.end())
		return false;

	w = *it;
	if (w.getType() != "other")
		return false;
	return true;
	
}

int Tokenizer::ConvertImmediateNumber(const string& arg, bool hex) {
	int idx = 0;
	string copyArg = arg;
	ToLower(copyArg);
	int val = 0;

	// Octal.
	//
	if ( (hex) || ((arg.length() >= 2) && (arg[0] == '0') && (arg[1] == 'x')) ){
		if (hex)
			idx = 0;
		else
			idx = 2;
		for (; idx < copyArg.length(); idx++) {
			if (copyArg[idx] >= 'a')
				val = val * 16 + copyArg[idx] - 'a' + 10;
			else
				val = val * 16 + copyArg[idx] - '0';
		}
	}
	else if (arg[idx] == '0') {
		for (idx = 1; idx < copyArg.length(); idx++) {
			val = val * 8 + copyArg[idx]-'0';
		}
	}
	else {
		int sign = 1;
		if (copyArg[0] == '+' || copyArg[0] == '-') {
			if (copyArg[0] == '-')
				sign = -1;
			idx = 1;
		}

		for (;idx < copyArg.length(); idx++)
				val = val * 10 + copyArg[idx] - '0';
		val *= sign;
	}
	return val;
}

bool Tokenizer::isOpcode(const string & arg, Wraper &w)
{
	set<Wraper>::iterator it;
	w.setName(arg);
	
	it = KEY_WORD.find(w);
	
	if (it == KEY_WORD.end())
		return false;

	w = *it;
	if (w.getType() != "instruction")
		return false;
	return true;
}

bool Tokenizer::isSection(const string & arg, Wraper &w, int line)
{
	int idx = 0;
	string newStr = ".";

	if (arg[idx++] != '.')
		return false;
	for (; idx < arg.size(); idx++) {
		if (arg[idx] != '.')
			newStr.append(1, arg[idx]);
		else break;
	}
	w.setName(newStr);
	set<Wraper>::iterator it = KEY_WORD.find(w);
	if (it == KEY_WORD.end())
		return false;

	w = *it;
	if (w.getType() != "section")
		return false;
	
	for (++idx;idx < arg.size(); idx++) {
		if (arg[idx] == '.')
			throw WrongInstructionFormat(line, "Wrong section name");
	}

	return true;

}


bool Tokenizer::isDirective(const string & arg, Wraper & w)
{
	set<Wraper>::iterator it;
	w.setName(arg);

	it = KEY_WORD.find(w);

	if (it == KEY_WORD.end())
		return false;

	w = *it;
	if (w.getType() != "directive")
		return false;
	return true;
}

void Tokenizer::SplitStringSpace(const string & arg, vector<string>& vTokens, bool special)
{

	string curStr = "";
	bool pushed = false;
	for (unsigned idx = 0; idx < arg.length(); idx++) {
		if ( (arg[idx] == ' ') || (arg[idx] == '\t') || ( (special) &&(arg[idx] == '='))) {
			if (curStr != "") {
				vTokens.push_back(curStr);
				curStr = "";
			}
		}
		else {
			curStr.append(string(1, arg[idx]));
			}

	}
	if (curStr != "")
		vTokens.push_back(curStr);
}

Tokenizer::~Tokenizer()
{
}

string Wraper::getType()
{
	return type;
}

string Wraper::getOpCode()
{
	return opCode;
}

string Wraper::getName()
{
	return name;
}

bool Wraper::getChangeS()
{
	return changeS;
}

void Wraper::setType(const string &s)
{
	type = s;
}

void Wraper::setOpCode(const string &s)
{
	opCode = s;
}

void Wraper::setName(const string & s)
{
	name = s;
}

void Wraper::setChangeS(bool b)
{
	changeS = b;
}

string Wraper::getInstructionName()
{
	return instructionName;
}


