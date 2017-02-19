#include "Assembler.h"
#include "AssemblingError.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <map>
#include <cstdlib>

const bool DEBUG = false;

using namespace std;



void Assembler::processPublicSecond()
{
	if (vTokens.size() == (1 + labelExists))
		throw WrongNumberOfArgumentsDirective(line, wraper.getName());
	for (; argNum < vTokens.size(); argNum++) {
		SymbolTableEntry* ste;
		if ((ste = symbolTable.FindByName(vTokens[argNum])) == NULL)
			throw WrongDirectiveFormat(line, wraper.getName() + " labels don't exist in code");
		ste->setVisibility("global");
	}
}

void Assembler::processIntSecond()
{
	const string& instrName = wraper.getInstructionName();

	if (vTokens.size() != (2 + labelExists))
		throw WrongNumberOfArgumentsInstruction(line, instrName);
	if (!tokenizer.isImmediate(vTokens[argNum]))
		throw ImmediateValueExcpected(line, instrName);

	int val = tokenizer.ConvertImmediateNumber(vTokens[argNum]);
	if ((val > 15) || (val < 0))
		throw TooBigImmediateValueEntered(line);
	else {
		string binary = tokenizer.convertDecimalToBinary(val, 4);

		output(tokenizer.convertBinaryToHex(wraper.getOpCode() + binary + string(20, '0'), line));
	}

}

void Assembler::processAddSubTestSecond()
{
	const string& instrName = wraper.getInstructionName();
	string binary  = wraper.getOpCode();

	if  (vTokens.size() != (3 + labelExists))
		throw WrongNumberOfArgumentsInstruction(line, instrName);
	if (!tokenizer.isRegister(vTokens[argNum], wraper))
		throw WrongInstructionFormat(line, " register is needed");
	if (wraper.getName() == "psw")
		throw WrongInstructionFormat(line, " psw not allowed");

	if (wraper.getType() == "reg")
		binary.append("0");
	binary.append(wraper.getOpCode());

	argNum++;
	if (!tokenizer.isRegister(vTokens[argNum], wraper)) {
		if (!tokenizer.isImmediate(vTokens[argNum])) {
			throw WrongInstructionFormat(line, " register or immediate is needed");
		}
		else {
			int val = tokenizer.ConvertImmediateNumber(vTokens[argNum]);
			binary.append("1" + tokenizer.convertDecimalToBinary(val, 18));
		}
	}
	else {
		if (wraper.getName() == "psw")
			throw WrongInstructionFormat(line, " psw not allowed");
		binary.append("0");
		if (wraper.getType() == "reg")
			binary.append("0");
		binary.append(wraper.getOpCode());
		binary.append(string(13, '0'));
	}
	if (DEBUG)
		output(binary);
	else
		output(tokenizer.convertBinaryToHex(binary, line));




}

void Assembler::
processMullDivSecond()
{
	const string& instrName = wraper.getInstructionName();
	string binary = wraper.getOpCode();

	if (vTokens.size() != (3 + labelExists))
		throw WrongNumberOfArgumentsInstruction(line, instrName);
	if (!tokenizer.isRegister(vTokens[argNum], wraper))
		throw WrongInstructionFormat(line, " register is needed");
	if (wraper.getType() == "specialreg")
		throw WrongInstructionFormat(line, " psw, sp, lr, and pc are not allowed");

	binary.append("0");
	binary.append(wraper.getOpCode());

	argNum++;
	if (!tokenizer.isRegister(vTokens[argNum], wraper)) {
		if (!tokenizer.isImmediate(vTokens[argNum])) {
			throw WrongInstructionFormat(line, " register or immediate is needed");
		}
		else {
			int val = tokenizer.ConvertImmediateNumber(vTokens[argNum]);
			binary.append("1" + tokenizer.convertDecimalToBinary(val, 18));
		}
	}
	else {
		if (wraper.getType() == "specialreg")
			throw WrongInstructionFormat(line, " psw, sp, lr, and pc are not allowed");
		binary.append("0");
		binary.append("0");
		binary.append(wraper.getOpCode());
		binary.append(string(13, '0'));
	}
	if (DEBUG)
		output(binary);
	else
		output(tokenizer.convertBinaryToHex(binary, line));



}

void Assembler::processAndOrNotTestSecond()
{
	const string& instrName = wraper.getInstructionName();
	string binary = wraper.getOpCode();

	if (vTokens.size() != (3 + labelExists))
		throw WrongNumberOfArgumentsInstruction(line, instrName);
	if (!tokenizer.isRegister(vTokens[argNum], wraper))
		throw WrongInstructionFormat(line, " register is needed");
	if ( (wraper.getName() == "psw") || (wraper.getName() == "pc") || (wraper.getName() == "lr"))
		throw WrongInstructionFormat(line, " psw, lr and pc are not allowed");

	if (wraper.getType() == "reg")
		binary.append("0");
	binary.append(wraper.getOpCode());

	argNum++;
	if (!tokenizer.isRegister(vTokens[argNum], wraper)) {
			throw WrongInstructionFormat(line, " register is needed");
	}
	else {
		if ((wraper.getName() == "psw") || (wraper.getName() == "pc") || (wraper.getName() == "lr"))
			throw WrongInstructionFormat(line, " psw, lr and pc are not allowed");
		if (wraper.getType() == "reg")
			binary.append("0");
		binary.append(wraper.getOpCode());
		binary.append(string(14, '0'));
	}
	if (DEBUG)
		output(binary);
	else
		output(tokenizer.convertBinaryToHex(binary, line));

}

void Assembler::processLdrStrSecond()
{
	const string& instrName = wraper.getInstructionName();
	string binary = wraper.getOpCode();

	if ( (vTokens.size() != (4 + labelExists)) && (vTokens.size() != (5 + labelExists)) )
		throw WrongNumberOfArgumentsInstruction(line, instrName);
	if (!tokenizer.isRegister(vTokens[argNum], wraper))
		throw WrongInstructionFormat(line, " register is needed");
	if (wraper.getName() == "psw")
		throw WrongInstructionFormat(line, " psw not allowed");
	// address register.
	//
	string addressReg = wraper.getName();
	if (wraper.getType() == "reg")
		binary.append("0");
	binary.append(wraper.getOpCode());

	argNum++;
	if (!tokenizer.isRegister(vTokens[argNum], wraper)) {
		throw WrongInstructionFormat(line, " register  is needed");
	}
	else {
		if (wraper.getType() == "reg")
			binary.append("0");
		binary.append(wraper.getOpCode());
	}

	argNum++;
	if (!tokenizer.isImmediate(vTokens[argNum])) {
		throw ImmediateValueExcpected(line);
	}
	int val = tokenizer.ConvertImmediateNumber(vTokens[argNum]);
	string immediate = tokenizer.convertDecimalToBinary(val, 10);

	argNum++;
	if (argNum != vTokens.size()) {
		if (!tokenizer.isOther(vTokens[argNum], wraper))
			throw WrongInstructionFormat(line, " prefixInc,... expected");
		if (addressReg == "pc")
			throw WrongInstructionFormat(line, " pc not alowed with increments");
		binary.append(wraper.getOpCode());
	}
	else {
		binary.append("000");
	}
	if (instrName == "ldr") {
		binary.append("1");
	}
	else {
		binary.append("0");
	}
	binary.append(immediate);

	if (DEBUG)
		output(binary);
	else
		output(tokenizer.convertBinaryToHex(binary, line));
}

void Assembler::processLdchLdclSecond()
{
	const string& instrName = wraper.getInstructionName();
	string binary = wraper.getOpCode();

	if (vTokens.size() != (3 + labelExists))
		throw WrongNumberOfArgumentsInstruction(line, instrName);
	if (!tokenizer.isRegister(vTokens[argNum], wraper))
		throw WrongInstructionFormat(line, " register is needed");

	if (wraper.getType() != "reg")
		throw WrongInstructionFormat(line, " no special register is allowed");
	binary.append(wraper.getOpCode());

	argNum++;
	if (instrName == "ldch") {
		binary.append("1");
	}
	else {
		binary.append("0");
	}
	binary.append(string(3, '0'));

	if (!tokenizer.isImmediate(vTokens[argNum])) {
		SymbolTableEntry* ste = symbolTable.FindByName(vTokens[argNum]);
		if ((ste == NULL) || (ste->getType() == "section")) {
			throw WrongInstructionFormat(line, " regular label or immediate is needed");
		}
		RellocationTableEntry* rte = NULL;
		if (ste->getType() == ".extern") {
			rte = new RellocationTableEntry(instrName, sizeOfSection, sections.back(), ste);
			binary.append(string(16, '0'));
		}
		else {
			rte = new RellocationTableEntry(instrName, sizeOfSection, sections.back(),  ste->getSection());
			int val = ste->getValue();
			binary.append(tokenizer.convertDecimalToBinary(val, 16));
		}
		rellocationTable.Insert(rte);
	}
	else {

		int val = tokenizer.ConvertImmediateNumber(vTokens[argNum]);
		binary.append(tokenizer.convertDecimalToBinary(val, 16));


	}
	if (DEBUG)
		output(binary);
	else
		output(tokenizer.convertBinaryToHex(binary, line));

}

void Assembler::processLdcSecond()
{
	const string& instrName = wraper.getInstructionName();
	string binary[2];
	for (int idx = 0; idx < 2; idx++)
		binary[idx] = wraper.getOpCode();

	if (vTokens.size() != (3 + labelExists))
		throw WrongNumberOfArgumentsInstruction(line, instrName);
	if (!tokenizer.isRegister(vTokens[argNum], wraper))
		throw WrongInstructionFormat(line, " register is needed");

	if (wraper.getType() != "reg")
		throw WrongInstructionFormat(line, " no special register is allowed");
	for (int idx = 0; idx < 2; idx++)
		binary[idx].append(wraper.getOpCode());

	argNum++;
	// ldcl
	//
	binary[0].append("0");
	// ldch
	//
	binary[1].append("1");

	for (int idx = 0; idx < 2; idx ++)
		binary[idx].append(string(3, '0'));

	if (!tokenizer.isImmediate(vTokens[argNum])) {
		SymbolTableEntry* ste = symbolTable.FindByName(vTokens[argNum]);
		if ((ste == NULL) || (ste->getType() == "section")) {
			throw WrongInstructionFormat(line, " regular label or immediate is needed");
		}
		RellocationTableEntry* rte[2] = { 0 };
		string instrNames[2] = { "ldclExt", "ldchExt" };
		if (ste->getType() == ".extern") {

			for (int idx = 0; idx < 2; idx++) {
				rte[idx] = new RellocationTableEntry(instrNames[idx], sizeOfSection, sections.back(), ste);
				binary[idx].append(string(16, '0'));
				if (idx == 0)
					sizeOfSection += 4;
			}
		}
		else {
			int val = ste->getValue();
			int higher = (val & ~((1 << 16) - 1)) >> 16;
			int lower = val & ((1 << 16) - 1);
			int vals[2] = { lower, higher };

			for (int idx = 0; idx < 2; idx++) {
				rte[idx] = new RellocationTableEntry(instrNames[idx], sizeOfSection, sections.back(), ste->getSection());
				binary[idx].append(tokenizer.convertDecimalToBinary(vals[idx], 16));
				if (idx == 0)
					sizeOfSection += 4;


			}

		}
		for (unsigned idx = 0; idx < 2; idx++)
			rellocationTable.Insert(rte[idx]);
	}
	else {
		int val = tokenizer.ConvertImmediateNumber(vTokens[argNum]);
		int higher = (val & ~((1 << 16) - 1)) >> 16;
		int lower = val & ((1 << 16) - 1);
		int vals[2] = { lower, higher };

		for (int idx = 0; idx < 2; idx++) {
			binary[idx].append(tokenizer.convertDecimalToBinary(vals[idx], 16));
			if (idx == 0)
				sizeOfSection += 4;
		}

	}
	//output(tokenizer.convertBinaryToHex(binary, line));
	for (int idx = 0; idx < 2; idx ++)
		if (DEBUG)
			output(binary[idx]);
		else
			output(tokenizer.convertBinaryToHex(binary[idx], line));
}



void Assembler::processCallSecond()
{
	const string& instrName = wraper.getInstructionName();
	string binary = wraper.getOpCode();

	if (vTokens.size() != (3 + labelExists))
		throw WrongNumberOfArgumentsInstruction(line, instrName);
	if (!tokenizer.isRegister(vTokens[argNum], wraper))
		throw WrongInstructionFormat(line, " register is needed");
	if (wraper.getType() == "reg")
		binary.append("0");
	binary.append(wraper.getOpCode());

	argNum++;
	if (!tokenizer.isImmediate(vTokens[argNum])) {
		throw WrongInstructionFormat(line, "immediate is needed");
	}
	else {
		int val = tokenizer.ConvertImmediateNumber(vTokens[argNum]);
		binary.append(tokenizer.convertDecimalToBinary(val, 19));
	}
	if (DEBUG)
		output(binary);
	else
		output(tokenizer.convertBinaryToHex(binary, line));

}

void Assembler::processInOutSecond()
{
	const string& instrName = wraper.getInstructionName();
	string binary = wraper.getOpCode();

	if (vTokens.size() != (3 + labelExists))
		throw WrongNumberOfArgumentsInstruction(line, instrName);
	if (!tokenizer.isRegister(vTokens[argNum], wraper))
		throw WrongInstructionFormat(line, " register is needed");

	if (wraper.getType() != "reg")
		throw WrongInstructionFormat(line, " no special register is allowed");
	binary.append(wraper.getOpCode());

	argNum++;
	if (!tokenizer.isRegister(vTokens[argNum], wraper)) {
		throw WrongInstructionFormat(line, " register  is needed");
	}
	else {
		if (wraper.getType() != "reg")
			throw WrongInstructionFormat(line, " no special register is allowed");
		binary.append(wraper.getOpCode());

		if (instrName == "in") {
			binary.append("1");
		}
		else {
			binary.append("0");
		}

		binary.append(string(15, '0'));
	}
	if (DEBUG)
		output(binary);
	else
		output(tokenizer.convertBinaryToHex(binary, line));

}

void Assembler::processMovShrShlSecond()
{

	const string& instrName = wraper.getInstructionName();
	string binary = wraper.getOpCode();

	if (((vTokens.size() != (3 + labelExists)) && (instrName == "mov"))
		|| ((vTokens.size() != (4 + labelExists)) && (instrName != "mov")))
		throw WrongNumberOfArgumentsInstruction(line, instrName);
	if (!tokenizer.isRegister(vTokens[argNum], wraper))
		throw WrongInstructionFormat(line, " register is needed");


	if (wraper.getType() == "reg")
		binary.append("0");
	binary.append(wraper.getOpCode());

	argNum++;
	if (!tokenizer.isRegister(vTokens[argNum], wraper)) {
		throw WrongInstructionFormat(line, " register  is needed");
	}
	else {
		if (wraper.getType() == "reg")
				binary.append("0");
		binary.append(wraper.getOpCode());

		argNum++;
		if (instrName != "mov") {
			if (!tokenizer.isImmediate(vTokens[argNum]))
				throw ImmediateValueExcpected(line);

			int val = tokenizer.ConvertImmediateNumber(vTokens[argNum]);
			binary.append(tokenizer.convertDecimalToBinary(val, 5));
			if (instrName == "shl")
				binary.append("1");
			else
				binary.append("0");
			binary.append(string(8, '0'));
		}
		else {
			binary.append(string(14, '0'));
		}
	}
	if (DEBUG)
		output(binary);
	else
		output(tokenizer.convertBinaryToHex(binary, line));
}

void Assembler::output(const string & s, bool sectionName)
{
	bool isData = sections.back()->getTypeOfSection() == ".data";
	if (sectionName) {
		*outFile << endl;
		*outFile << s << endl;
		cntNibble = 0;
	}
	else if (isData) {
		for (unsigned idx = 0; idx < s.length(); idx++) {
			*outFile << s[idx];
			cntNibble++;
			if ((cntNibble % 2) == 0)
				*outFile << " ";

			if (cntNibble == 32) {
				*outFile << endl;
				cntNibble = 0;
			}
		}
	}
	else if (sections.back()->getTypeOfSection() == ".text") {
		for (unsigned idx = 0; idx < s.length(); idx++) {
			*outFile << s[idx];
			cntNibble++;
			if ((cntNibble % 8) == 0)
				*outFile << " ";
			if (cntNibble == 32) {
				*outFile << endl;
				cntNibble = 0;
			}
		}
	}

}

Assembler::Assembler(ifstream* _inFile, ofstream* _outFile) : inFile(_inFile), outFile(_outFile)
{

}

void printCurentTime() {
	time_t rawtime;
	struct tm * timeinfo;
	char buffer[80];

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer, 80, "%d-%m-%Y %I:%M:%S", timeinfo);
	std::string str(buffer);

	std::cout << str;
}

void Assembler::FirstPass()
{



	lastLabel = false;
	line = 0;
	labelExists = false;
	line = 0;
	sizeOfSection = 0;
	end = false;

	// Reads untill end of file.
	//

	cout << "***********************FIRST PASS ********************** " << endl;
	while (!inFile->eof() && !end) {
		// For testing purposes only.
		//
		try {
			// Reads first line of file.
			//
			getline(*inFile, curLine);
			cout << "In line" << setw(3) << (++line) << "  : " << curLine;
			vTokens.clear();
			tokenizer.SplitString(curLine, vTokens, line - 1, labelExists);



			cout << endl << "Tokens:";
			for (int idx = 0; idx < vTokens.size(); idx++)
				cout << endl << setw(3) << idx << " " << vTokens[idx] << " ";
			cout << endl << "TokenEnd" << endl;

			// Empty line.
			//
			if (vTokens.size() == 0)
				continue;

			argNum = 0;
			if (labelExists) {
				if (tokenizer.IsKeyWord(vTokens[0]))
					throw WrongInstructionFormat(line, "Wrong label name");
				processLabel();
				if (vTokens.size() == 1)
					continue;

			}


			if (vTokens.size() > argNum + labelExists) {
				if (tokenizer.isSection(vTokens[argNum], wraper, line)) {

					processSectionFirst(wraper);

				}
				else if (tokenizer.isDirective(vTokens[argNum], wraper)) {
					if ( (wraper.getName() != ".skip") && (sections.size() != 0)
						&&(sections.back()->getTypeOfSection() == ".bss") && (wraper.getName() != ".end"))
					{
						throw WrongDirectiveFormat(line, wraper.getName() + "Not alowed in bss");
					}
					if (wraper.getName() == ".end") {
						processEndFirst();
						end = true;
					}
					else if (wraper.getName() == ".skip") {
						processSkipFirst();
					}
					else if (wraper.getName() == ".char") {
						processCharFirst();
					}
					else if (wraper.getName() == ".word") {
						processWordFirst();
					}
					else if (wraper.getName() == ".long") {
						processLongFirst();
					}
					else if (wraper.getName() == ".align") {
						processAlignFirst();
					}
					else if (wraper.getName() == ".extern") {
						processFirstExtern();
					}
					else if (wraper.getName() == ".public") {
						processFirstPublic();
					}
				}
				else if (tokenizer.isOpcode(vTokens[argNum], wraper)) {
					if ((sections.size() == 0) || (sections.back()->getTypeOfSection() != ".text"))
						throw InstructionNotInTextSection(line);
					UpdateLabels();
					sizeOfSection += 4;
					if (wraper.getInstructionName() == "ldc")
					sizeOfSection += 4;
					lastLabel = false;
				}
				else {
					throw WrongInstructionFormat(line, "unknown instruction");
				}
			}
			else {
				throw WrongInstructionFormat(line, " wrong number of arguments");
			}

			}
			catch (AssemblingError& e) {
				cout << e.GetError() << "  line : " << e.getLine() << endl;
				printCurentTime();
				vTokens.clear();
				cout << endl << endl;;
				exit(e.exitCodeReturn());
			}

	}
	if (lastLabel)
		throw LabelDoesntHaveInstruction(line);
	if (!end)
		throw EndDoesNotExists(line);
	cout << "***********************FIRST PASS ********************** " << endl;
}


void Assembler::processSectionFirst(Wraper& wraper)
{
	if (lastLabel)
		throw WrongInstructionFormat(line, "Wrong label position");
	if (vTokens.size() >= 2)
		throw WrongInstructionFormat(line, "No data is allowed after section name");
	// Add to symbol table current section.
	// getName returns type of section
	//
	SymbolTableEntry* ste = new SymbolTableEntry(vTokens[argNum], "local", 0, wraper.getName());
	symbolTable.Insert(ste, line);
	if (sections.size() != 0) {
		sections.back()->setSize(sizeOfSection);
	}
	sections.push_back(ste);
	sizeOfSection = 0;
}

void Assembler::processLabel()
{
	lastLabel = true;
	if (sections.size() == 0)
		throw LabelDoesntHaveInstruction(line, " label must be in section");

	SymbolTableEntry* ste = symbolTable.FindByName(vTokens[argNum]);

	ste = new SymbolTableEntry(vTokens[argNum], "local", -1);
	symbolTable.Insert(ste, line);
	// Niz labela koje treba azurirati kad se naidje na instrukciju.
	//
	labels.push_back(ste);

	argNum++;
}

void Assembler::UpdateLabels()
{
	for (int idx = 0; idx < labels.size(); idx++) {
		labels[idx]->setValue(sizeOfSection);
		labels[idx]->setSection(sections.back());
	}
	labels.clear();
}

void Assembler::processEndFirst()
{
	if (sections.size() == 0)
		throw DirectiveNotInSection(line);
	if (lastLabel)
		throw LabelDoesntHaveInstruction(line, " label must be in section");
	if (vTokens.size() >= 2)
		throw WrongInstructionFormat(line, "No data is allowed after .end in the same line");
	// .end is allowed.
	//
	if (sections.size() != 0) {
		sections.back()->setSize(sizeOfSection);
	}
	end = true;
}

void Assembler::processSkipFirst() {
	if (sections.size() == 0)
		throw DirectiveNotInSection(line);
	if (vTokens.size() != (2 + labelExists)) {
		throw WrongNumberOfArgumentsDirective(line, ".skip");
	}
	if (lastLabel)  UpdateLabels();

	argNum++;
	if (!tokenizer.isImmediate(vTokens[argNum]))
		throw ImmediateValueExcpected(line, ".skip");

	int val = tokenizer.ConvertImmediateNumber(vTokens[argNum]);
	if ( (val <= 0))
		throw TooBigImmediateValueEntered(line);
	sizeOfSection += val;
	lastLabel = false;

}

void Assembler::processWordFirst()
{
	if (sections.size() == 0)
		throw DirectiveNotInSection(line);
	if (sections.back()->getTypeOfSection() != ".data")
		throw WrongDirectiveFormat(line, " .word only allowed in .data section");
	if (vTokens.size() < (2 + labelExists))
		throw WrongNumberOfArgumentsDirective(line, ".word");
	if (lastLabel)  UpdateLabels();

	argNum++;
	for (; argNum < vTokens.size(); argNum++) {
		if (!tokenizer.isImmediate(vTokens[argNum]))
			throw ImmediateValueExcpected(line, ".word");
		sizeOfSection += 2;
	}
	lastLabel = false;

}

void Assembler::processCharFirst()
{
	if (sections.size() == 0)
		throw DirectiveNotInSection(line);
	if (sections.back()->getTypeOfSection() != ".data")
		throw WrongDirectiveFormat(line, " .char only allowed in .data section");
	if (vTokens.size() < (2 + labelExists))
		throw WrongNumberOfArgumentsDirective(line, ".char");
	if (lastLabel)  UpdateLabels();

	argNum++;
	for (; argNum < vTokens.size(); argNum++) {
		if (!tokenizer.isImmediate(vTokens[argNum]) && !tokenizer.isChar(vTokens[argNum]))
			throw ImmediateValueExcpected(line, ".char");
		sizeOfSection += 1;
	}
	lastLabel = false;

}

void Assembler::processLongFirst()
{
	if (sections.size() == 0)
		throw DirectiveNotInSection(line);
	if (sections.back()->getTypeOfSection() != ".data")
		throw WrongDirectiveFormat(line, " .long in data section only allowed");
	// Sredi proveru za brojeve, da mogu i labele.
	//
	if (vTokens.size() < (2 + labelExists))
		throw WrongNumberOfArgumentsDirective(line, wraper.getName());
	if (lastLabel)  UpdateLabels();

	argNum++;
	for (; argNum < vTokens.size(); argNum++) {
		if (vTokens[argNum] == ",")
			sizeOfSection += 4;
	}
	sizeOfSection += 4;
	lastLabel = false;

}

void Assembler::processAlignFirst()
{
	if (sections.size() == 0)
		throw DirectiveNotInSection(line);
	if (vTokens.size() != (2 + labelExists))
		throw WrongNumberOfArgumentsDirective(line, wraper.getName());
	argNum++;
	if (!tokenizer.isImmediate(vTokens[argNum]))
		throw ImmediateValueExcpected(line, wraper.getName());

	int val = tokenizer.ConvertImmediateNumber(vTokens[argNum]);

	if ((val <= 0) || (val > 15))
		throw TooBigImmediateValueEntered(line);

	// It's already aligned.
	//
	if ((sizeOfSection % (1 << val)) == 0)
		return;

	int value = sizeOfSection >> val;
	value++;
	value <<= val;
	sizeOfSection = value;
}

void Assembler::processFirstExtern()
{
	if (sections.size() > 0) {
		throw DirectiveInSection(line, wraper.getName());
	}
}

void Assembler::processFirstPublic()
{
	if (sections.size() > 0) {
		throw DirectiveInSection(line, wraper.getName());
	}
}




void Assembler::SecondPass()
{
	cout << "***********************SECOND PASS ********************** " << endl;

	lastLabel = false;
	line = 0;
	labelExists = false;
	line = 0;
	sizeOfSection = 0;
	end = false;
	sections.clear();

	inFile->clear();
	inFile->seekg(0, ios::beg);
	sections.clear();

	// Reads untill end of file.
	//
	while (!inFile->eof() && !end) {
		// For testing purposes only.
		//
		try {

			// Reads first line of file.
			//
			getline(*inFile, curLine);
			cout << "In line" << setw(3) << (++line) << "  : " << curLine;
			vTokens.clear();
			tokenizer.SplitString(curLine, vTokens, line - 1, labelExists);



			cout << endl << "Tokens:";
			for (int idx = 0; idx < vTokens.size(); idx++)
				cout << endl << setw(3) << idx << " " << vTokens[idx] << " ";
			cout << endl << "TokenEnd" << endl;

			// Empty line.
			//
			if (vTokens.size() == 0)
				continue;

			argNum = 0;
			if (labelExists) {
				if (vTokens.size() == 1)
					continue;
				argNum++;
			}

			if (vTokens.size() > argNum + labelExists) {
				if (tokenizer.isSection(vTokens[argNum], wraper, line)) {

					processSectionSecond(wraper);

				}
				else if (tokenizer.isDirective(vTokens[argNum], wraper)) {
					argNum++;
					if (wraper.getName() == ".end") {
						processEndSecond();
					}
					else if (wraper.getName() == ".skip") {
						processSkipSecond();
					}
					else if (wraper.getName() == ".char") {
						processCharSecond();
					}
					else if (wraper.getName() == ".word") {
						processWordSecond();
					}
					else if (wraper.getName() == ".long") {
						processLongSecond();
					}
					else if (wraper.getName() == ".align") {
						processAlignSecond();
					}
					else if (wraper.getName() == ".extern") {
						processExternSecond();
					}
					else if (wraper.getName() == ".public")
					{
						processPublicSecond();
					}
				}
				else if (tokenizer.isOpcode(vTokens[argNum], wraper)) {
					const string& instrName = wraper.getInstructionName();
					argNum++;
					if (instrName == "int") {
						processIntSecond();
					}
					else if ((instrName == "add") || (instrName == "sub") || (instrName == "cmp")) {
						processAddSubTestSecond();
					}
					else if ((instrName == "mul") || (instrName == "div")) {
						processMullDivSecond();
					}
					else if ((instrName == "and") || (instrName == "or") || (instrName == "not")
						|| (instrName == "test")) {
						processAndOrNotTestSecond();
					}
					else if ( (instrName == "ldr") || (instrName == "str"))  {
						processLdrStrSecond();
					}
					else if (instrName == "call")
						processCallSecond();
					else if ((instrName == "in") || (instrName == "out")) {
						processInOutSecond();
					}
					else if ((instrName == "mov") || (instrName == "shr")
						|| (instrName == "shl")) {
						processMovShrShlSecond();
					}
					else if ((instrName == "ldch") || (instrName == "ldcl")) {
						processLdchLdclSecond();
					}

					else if (instrName == "ldc") {
						processLdcSecond();
					}
					sizeOfSection += 4;
				}
				else {
					throw WrongInstructionFormat(line, "unknown instruction");
				}
			}
			else {
				throw WrongInstructionFormat(line, " wrong number of arguments");
			}

		}
		catch (AssemblingError& e) {
			cout << e.GetError() << "  line : " << e.getLine() << endl;
			printCurentTime();
			vTokens.clear();
			cout << endl << endl;;
			exit(e.exitCodeReturn());
		}

	}
	if (lastLabel)
		throw LabelDoesntHaveInstruction(line);
	if (!end)
		throw EndDoesNotExists(line);
	cout << "***********************SECOND PASS ********************** " << endl;
}

void Assembler::OutputSymbolTable()
{
	unsigned indent = 15;
	vector<SymbolTableEntry*> symEntries = symbolTable.GetVectorOfEntries();
	*outFile << endl << "*******************Symbol table *********************" << endl;


	*outFile << setw(indent) << "idx";
	*outFile << setw(indent) << "Name";

	*outFile << setw(indent) << "section";

	*outFile << setw(indent) << "type";
	*outFile << setw(indent) << "sectionT";
	*outFile << setw(indent) << "size";
	*outFile << setw(indent) << "value";
	*outFile << setw(indent) << "visibility";
	*outFile << endl;

	for (unsigned idx = 0; idx < symEntries.size(); idx ++) {
		*outFile << setw(indent) << symEntries[idx]->getIdx();
		*outFile << setw(indent) << symEntries[idx]->getName();
		if (symEntries[idx]->getType() == "section")
			*outFile << setw(indent) << symEntries[idx]->getName();
		else if (symEntries[idx]->getType() == ".extern")
			*outFile << setw(indent) << "UNDEF";
		else
			*outFile << setw(indent) << symEntries[idx]->getSection()->getName();
		*outFile << setw(indent) << symEntries[idx]->getType();
		*outFile << setw(indent) << symEntries[idx]->getTypeOfSection();
		*outFile << setw(indent) << symEntries[idx]->getSize();
		*outFile << setw(indent) << symEntries[idx]->getValue();
		*outFile << setw(indent) << symEntries[idx]->getVisibility();
		*outFile << endl;
	}
}

void Assembler::OutputRellocationTable()
{
	unsigned indent = 18;
	vector< RellocationTableEntry*> relEntries = rellocationTable.getRellocationVector();



	for (unsigned idxSection = 0; idxSection < sections.size(); idxSection++) {
		if (sections[idxSection]->getTypeOfSection() == ".bss")
			continue;
		*outFile << "rel" + sections[idxSection]->getName() << endl;

		*outFile << setw(indent) << "offset";
		*outFile << setw(indent) << "type";
		*outFile << setw(indent) << "idx";
		*outFile << setw(indent) << "ref section";
		*outFile << endl;
		for (unsigned idx = 0; idx < relEntries.size(); idx++) {
			if (relEntries[idx]->getSection() == sections[idxSection]) {
				*outFile << setw(indent) << relEntries[idx]->getOffsetSection();
				*outFile << setw(indent) << relEntries[idx]->getType();
				*outFile << setw(indent) << relEntries[idx]->getEntry()->getIdx();
				*outFile << setw(indent) << relEntries[idx]->getEntry()->getName();
				*outFile << endl;
			}
		}
	}
}

void Assembler::processSectionSecond(Wraper & wraper)
{
	SymbolTableEntry*  ste = symbolTable.FindByName(vTokens[argNum]);
	sections.push_back(ste);
	output(vTokens[argNum],  true);
	sizeOfSection = 0;

}

void Assembler::processEndSecond()
{
	end = true;
}

void Assembler::processSkipSecond()
{
	if (vTokens.size() != (2 + labelExists)) {
		throw WrongNumberOfArgumentsDirective(line, ".skip");
	}

	if (!tokenizer.isImmediate(vTokens[argNum]))
		throw ImmediateValueExcpected(line, ".skip");

	int val = tokenizer.ConvertImmediateNumber(vTokens[argNum]);
	if ((val <= 0))
		throw TooBigImmediateValueEntered(line);
	string str = "";
	for (int idx = 0; idx < val; idx++)
		str.append("00");
	output(str);
	sizeOfSection += val;
}

void Assembler::processWordSecond()
{
	for (;argNum < vTokens.size(); argNum++) {
		int val = tokenizer.ConvertImmediateNumber(vTokens[argNum]);
		string binary = tokenizer.convertBinaryToHex(tokenizer.convertDecimalToBinary(val, WORD_SIZE), line);
		tokenizer.littleEndian(binary);
		output(binary);
		sizeOfSection += 2;
	}
}


void Assembler::processCharSecond()
{
	for (;argNum < vTokens.size(); argNum++) {
		int val;
		if (tokenizer.isChar(vTokens[argNum])) {
			val = tokenizer.ConvertChar(vTokens[argNum]);
		}
		else {
			val = tokenizer.ConvertImmediateNumber(vTokens[argNum]);
		}
		string binary = tokenizer.convertBinaryToHex(tokenizer.convertDecimalToBinary(val, CHAR_SIZE), line);
		output(binary);
		sizeOfSection += 1;
	}
}

void Assembler::processLongSecond()
{
	int val = 0;
	map <SymbolTableEntry*, int> m;
	map <SymbolTableEntry*, int> mCnt;
	int cnt = 0;
	int last = 1;
	SymbolTableEntry* lastSection = NULL;
	SymbolTableEntry* externSym = NULL;
	for (; argNum < vTokens.size(); argNum++) {
		if (vTokens[argNum] == ",") {

			if ((mCnt.find(NULL) != mCnt.end()) && ((mCnt.size() > 1) || (val != 0))) {
				throw WrongDirectiveFormat(line, " .extern");
			}
			for (map<SymbolTableEntry*, int>::iterator it = mCnt.begin(); it != mCnt.end(); it++)
			{

				if ( (it->second > 1) || (it->second < 0)) {
					throw WrongInstructionFormat(line, " too much labels");
				}
				else if (it->second == 1)
				{
					cnt++;
					lastSection = it->first;
				}
				val += m[it->first];
			}
			if ((cnt != 0) && (cnt != 1)) {
				throw WrongInstructionFormat(line, " too much labels");
			}
			string binary = tokenizer.convertBinaryToHex(tokenizer.convertDecimalToBinary(val, LONG_SIZE), line);
			tokenizer.littleEndian(binary);
			output(binary);
			if (cnt== 1) {
				RellocationTableEntry* rte = NULL;
				if (mCnt.find(NULL) != mCnt.end()) {
					rte = new RellocationTableEntry(".long", sizeOfSection, sections.back(), externSym);
				}
				else {
					rte = new RellocationTableEntry(".long", sizeOfSection, sections.back(), lastSection);
				}
				rellocationTable.Insert(rte);
			}
			sizeOfSection += 4;
			m.clear();
			mCnt.clear();
			cnt = 0;
			val = 0;
			last = 1;
		}
		else if (vTokens[argNum] == "-") {
			last = -1;
		}
		else if (vTokens[argNum] == "+") {
			last = 1;
		}
		else if (tokenizer.isImmediate(vTokens[argNum])) {
			val += last * tokenizer.ConvertImmediateNumber(vTokens[argNum]);
		}
		else {
			SymbolTableEntry* ste = symbolTable.FindByName(vTokens[argNum]);
			if ((ste == NULL) || ( (ste->getType() != "global") && (ste->getType() != "local") &&(ste->getType() != ".extern") ) )
				throw WrongDirectiveFormat(line, " regular label is needed");
			SymbolTableEntry* section = ste->getSection();


			if (m.find(section) == m.end()) {
				if (ste->getType() == ".extern") {
					m[section] = 0;
					mCnt[section] = 1;
					externSym = ste;
				}
				else {
					m[section] = last * ste->getValue();
					mCnt[section] = last;
				}
			}
			else {
				if (ste->getType() == ".extern") {
					throw WrongDirectiveFormat(line, "Not allowed extern two times");
				}
				else {
					m[section] += last * ste->getValue();
					mCnt[section] += last;
				}

			}
		}
	}

	if ((mCnt.find(NULL) != mCnt.end()) && ((mCnt.size() > 1) || (val != 0))) {
		throw WrongDirectiveFormat(line, " .extern");
	}
	for (map<SymbolTableEntry*, int>::iterator it = mCnt.begin(); it != mCnt.end(); it++)
	{

		if ((it->second > 1) || (it->second < 0)) {
			throw WrongInstructionFormat(line, " too much labels");
		}
		else if (it->second == 1)
		{
			cnt++;
			lastSection = it->first;
		}
		val += m[it->first];
	}
	if ((cnt != 0) && (cnt != 1)) {
		throw WrongInstructionFormat(line, " too much labels");
	}
	string binary = tokenizer.convertBinaryToHex(tokenizer.convertDecimalToBinary(val, LONG_SIZE), line);
	tokenizer.littleEndian(binary);
	output(binary);
	if (cnt == 1) {
		RellocationTableEntry* rte = NULL;
		if (mCnt.find(NULL) != mCnt.end()) {
			rte = new RellocationTableEntry(".long", sizeOfSection, sections.back(), externSym);
		}
		else {
			rte = new RellocationTableEntry(".long", sizeOfSection, sections.back(), lastSection);
		}
		rellocationTable.Insert(rte);
	}
	sizeOfSection += 4;
}

void Assembler::processAlignSecond()
{
	int val = tokenizer.ConvertImmediateNumber(vTokens[argNum]);

	// It's already aligned.
	//
	if ((sizeOfSection % (1 << val)) == 0)
		return;

	int value = sizeOfSection >> val;
	value++;
	value <<= val;
	for (int idx = sizeOfSection; idx < value; idx++)
		output("00");
	sizeOfSection = value;
}

void Assembler::processExternSecond()
{
	if (vTokens.size() == (1 + labelExists)) {
		throw WrongNumberOfArgumentsDirective(line, ".extern");
	}
	// Prepusticu korisniku da odlucuje o "regularnim" imenima labela.
	//
	for (; argNum < vTokens.size(); argNum++)
		if (tokenizer.IsKeyWord(vTokens[argNum]))
			throw WrongInstructionFormat(line, "Label cannot be the key word");
		else {
			SymbolTableEntry* ste = new SymbolTableEntry(vTokens[argNum], "global", -1);
			symbolTable.Insert(ste, line);
		}
}


