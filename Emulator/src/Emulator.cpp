#include "Emulator.h"

#include <fstream>
#include <vector>
#include <iostream>
#include <signal.h>

#include "EmulatorError.h"
using namespace std;

void printCurentTime() {

}

void Emulator::ReadObjectFile(const string & nameOfFile)
{
	last = "";
	lastName = "";
	lastSection = NULL;

	ifstream inFile(nameOfFile);
	if (!inFile.is_open())
		throw InputFileProblem(-1);
	line = 0;
	try {
		while (!inFile.eof()) {
			line++;
			getline(inFile, curLine);


			tokenizer.SplitStringSpace(curLine, vTokens);
			if (vTokens.size() == 0)
				continue;
			if (vTokens[0][0] == '.') {
				AddSectionSymbol(nameOfFile);
			}
			// Rellocation.
			//
			else if (vTokens[0][0] == 'r') {
				AddRellocationTable(nameOfFile);
				getline(inFile, curLine);
				line++;
			}
			// Symbol table.
			//
			else if (vTokens[0][0] == '*') {
				last = "symTable";
				//Header
				//
				getline(inFile, curLine);
				line++;
			}
			else {
				if (last == "section") {
					for (unsigned idx = 0; idx < vTokens.size(); idx++)
						mSection[lastName]->AddSectionContent(vTokens[idx]);
				}
				else if (last == "rel") {
					AddRellocationEntry(nameOfFile);
				}
				else if (last == "symTable") {
					AddSymbolTableEntry(nameOfFile);
				}
			}
			vTokens.clear();
		}
	}
	catch (EmulatorError& e) {
		cout << e.GetError() << "  line : " << e.getLine() << endl;
		printCurentTime();
		vTokens.clear();
		cout << endl << endl;;
		exit(e.exitCodeReturn());
	}
}

void Emulator::AddSectionSymbol(const string& nameOfFile)
{
	if (mSection.find(vTokens[0]) != mSection.end())
		throw DuplicateSectionSymbol(line, nameOfFile);
	Section* section = new Section(vTokens[0]);

	mSection[section->getName()] = section;
	last = "section";
	lastName = section->getName();
	lastSection = section;
}

void Emulator::AddRellocationTable(const string& nameOfFile)
{
	last = "rel";
	lastName = vTokens[0].erase(0, 3);
	if (mSection.find(lastName) == mSection.end())
		throw ChangedObjFile(line, "relation data" + nameOfFile);
	lastSection = mSection[lastName];

}

void Emulator::UpdateSectionAddress()
{
	if (mSection.find(vTokens[0]) == mSection.end())
		throw SectionDoesNotExists(line, loaderName);
	Section* curSection = mSection[vTokens[0]];
	curAddress = mLoadScript["."]->getValue();
	if (curSection->hasBeginAddress())
		throw BeginAddressAlreadySet(line, loaderName);
	curSection->setBeginAddress(curAddress);
	curAddress += curSection->getSize();
	mLoadScript["."]->setValue(curAddress);
	// Sve sekcije koje su stavljene.
	//
	vSection.push_back(curSection);
	// Update globals in this section.
	//
}

void Emulator::AddRellocationEntry(const string & nameOfFile)
{

		if (vTokens.size() != 4) {
			throw ChangedObjFile(line, "relation data" + nameOfFile);
		}

		if (!tokenizer.isImmediate(vTokens[0]) && !tokenizer.isImmediate(vTokens[2]))
			throw ChangedObjFile(line, "relation data" + nameOfFile);

		int offset = tokenizer.ConvertImmediateNumber(vTokens[argNum]);
		string type = vTokens[1];
		string symbolReferenced = vTokens[3];

		RellocationTableEntry* rte = NULL;
		if (symbolReferenced[0] == '.')
			rte = new RellocationTableEntry(type, offset, lastSection->getSymbolTableEntry(), mSection[symbolReferenced]->getSymbolTableEntry());
		else {
			rte = new RellocationTableEntry(type, offset, lastSection->getSymbolTableEntry(), mExtern[symbolReferenced]);
		}
		mSection[lastName]->getRellocationTable().Insert(rte);

}

void Emulator::AddSymbolTableEntry(const string & nameOfFile)
{
	if (vTokens.size() != 8) {
		throw ChangedObjFile(line, "relation data" + nameOfFile);
	}

	if (!tokenizer.isImmediate(vTokens[0]) && !tokenizer.isImmediate(vTokens[5]) && !tokenizer.isImmediate(vTokens[6]))
		throw ChangedObjFile(line, "relation data" + nameOfFile);

	if ((vTokens[7] != "local") && (vTokens[7] != "global"))
		throw ChangedObjFile(line, "relation data" + nameOfFile);

	string name = vTokens[1];
	string section = vTokens[2];
	string type = vTokens[3];
	string sectionT = vTokens[4];
	int size = tokenizer.ConvertImmediateNumber(vTokens[5]);
	int value = tokenizer.ConvertImmediateNumber(vTokens[6]);
	string visibility = vTokens[7];


	SymbolTableEntry* ste = new SymbolTableEntry(name, visibility, value, sectionT, size);
	if (type == "section") {
		mSection[name]->setSymbolTableEntry(ste);
		mSection[name]->setSize(size);
		lastSection = mSection[name];
	}
	else if (type == "global") {
		// Zato sto su mi svi simboli u tabeli redom stavljeni
		//
		lastSection->getSymbolTable().Insert(ste, line);
		if (mGlobal.find(name) != mGlobal.end()) {
			throw DuplicateGlobalSymbol(line, nameOfFile);
		}
		mGlobal[name] = ste;
		mGlobal[name]->setSection(lastSection->getSymbolTableEntry());
	}
	else if (type == ".extern") {
		if (mExtern.find(name) == mExtern.end())
			mExtern[name] = new SymbolTableEntry(name, visibility, value, sectionT, size);
	}
	else {
		delete ste;
	}
}




void Emulator::loadFiles()
{
	for (unsigned idx = 0; idx < asOutputNames.size(); idx++) {
		ReadObjectFile(asOutputNames[idx]);
	}
}

void Emulator::readLoadScript()
{

	ifstream inFile(loaderName);
	if (!inFile.is_open())
		throw InputFileProblem(-1);
	line = 0;

	SymbolTableEntry* ste = new SymbolTableEntry(".", "global", 0);
	mLoadScript["."] = ste;

	try {
		while (!inFile.eof())
		{
			line++;
			argNum = 0;
			getline(inFile, curLine);

			tokenizer.SplitStringSpace(curLine, vTokens, true);
			if (vTokens.size() == 0)
				continue;
			lastName = vTokens[0];

			// Section case.
			//
			if (vTokens.size() == 1)
			{
				UpdateSectionAddress();
			}
			else  if (lastName == ".") {
				int val = ParseRightSide(curLine);
				if (val < mLoadScript["."]->getValue())
					throw AddressLess(line, loaderName);
				else
					mLoadScript["."]->setValue(val);

			}
			else {
				if (mGlobal.find(lastName) != mGlobal.end())
					throw DuplicateGlobalSymbol(line, loaderName);
				SymbolTableEntry* ste = new SymbolTableEntry(lastName, "global", ParseRightSide(curLine));
				mGlobal[lastName] = ste;
				mLoadScript[lastName] = ste;
			}
			vTokens.clear();
		}

	}
	catch (EmulatorError& e) {
		cout << e.GetError() << "  line : " << e.getLine() << endl;
		printCurentTime();
		vTokens.clear();
		cout << endl << endl;;
		exit(e.exitCodeReturn());
	}
}

void Emulator::resolveExtern()
{
	for (map<string, SymbolTableEntry*>::iterator it = mExtern.begin(); it != mExtern.end(); it++) {
		if (mGlobal.find(it->second->getName()) == mGlobal.end()) {
			throw UnresolvedSymbol(-1, it->second->getName());
		}
	}
}

void Emulator::sortSections()
{
	vSection.clear();
	for (map<string, Section*>::iterator it = mSection.begin(); it != mSection.end(); it++) {
		vSection.push_back(it->second);
	}
	for (unsigned idx = 0; idx < vSection.size() - 1; idx++) {
		for (unsigned idxIn = idx + 1; idxIn < vSection.size(); idxIn++) {
			if (*vSection[idxIn] < *vSection[idx]) {
				Section* tmp = vSection[idx];
				vSection[idx] = vSection[idxIn];
				vSection[idxIn] = tmp;
			}
		}
	}
	for (unsigned idx = 0; idx < vSection.size(); idx++) {
		if (!vSection[idx]->hasBeginAddress()) {
			vSection[idx]->setBeginAddress(mLoadScript["."]->getValue());
			mLoadScript["."]->setValue(mLoadScript["."]->getValue() + vSection[idx]->getSize());
		}
		vector <SymbolTableEntry*> stv = vSection[idx]->getSymbolTable().GetVectorOfEntries();
		for (unsigned idxSt = 0; idxSt < stv.size(); idxSt++)
			stv[idxSt]->setValue(stv[idxSt]->getValue() + vSection[idx]->getBeginAddress());
	}
}

void Emulator::resolveRellocations()
{
	for (unsigned idx = 0; idx < vSection.size(); idx ++) {
		vector <RellocationTableEntry*> rt = vSection[idx]->getRellocationTable().getRellocationVector();

		for (unsigned idxRel = 0; idxRel < rt.size(); idxRel++) {

			RellocationTableEntry* rte = rt[idxRel];
			if (rte->getEntry()->getType() == ".extern") {
					vSection[idx]->ResolveSymbol(rte, mGlobal[rte->getEntry()->getName()]->getValue(), tokenizer);
			}
			else {
				vSection[idx]->ResolveSymbol(rte, mSection[rte->getEntry()->getName()]->getBeginAddress(), tokenizer);
			}
			if (rte->getType() == "ldclExt") {
				idxRel++;
			}
		}
	}
}

void Emulator::loadInMemory()
{
	if ((mGlobal.find("main") == mGlobal.end()) || (mLoadScript.find("global") != mLoadScript.end())) {
		throw MainDoesNotExist(-1);
	}


	// Idx, counts num of sections.
	//
	for (unsigned idx = 0; idx < vSection.size(); idx++) {

		int size = vSection[idx]->getSize();
		// idxsec, counts bytes in section.
		//
		for (int idxSec = 0; idxSec < size; idxSec++) {
			string byteS = vSection[idx]->getContent().substr(idxSec * 2,  2);
			byte b = tokenizer.ConvertImmediateNumber(byteS, true);
			memory.WriteRamByte(vSection[idx]->getBeginAddress() + idxSec, b);
		}
	}
}

void Emulator::initSystem()
{
	ivt.InitTable();
	cpu.regs[CPU::LR] = mGlobal["main"]->getValue();
	cpu.regs[CPU::PSW] = 0;
	cpu.regs[CPU::SP] = 0xffffff;

	for (int idx = 0; idx < CPU::NUM_NORMAL_REGS; idx++)
		cpu.regs[idx] = 0;


	cpu.setTimerFlag(true);
	cpu.Push(0);
	cpu.Push(cpu.regs[CPU::PSW]);

	// Entry routine.
	//
	cpu.SetGlobalInterrupt(true);
	cpu.regs[CPU::PC] = ivt.getRoutine(0);
	endEmulate = false;
	pthread_mutex_init(&CPU::mutexInterrupt, NULL);
	memory.WriteIOInt(CPU::KEY_PRESSED, 0);

}

void Emulator::run()
{
	pthread_t keyBoardThread;
	pthread_t timerThread;
	pthread_attr_t defAtr;

	pthread_attr_init(&defAtr);
	pthread_create(&keyBoardThread, &defAtr, CPU::KeyboardHandler, NULL);
	pthread_create(&timerThread, &defAtr, CPU::Timer, NULL);
	/*if (pthread_join(keyBoardThread, NULL)) {

		fprintf(stderr, "Error joining thread\n");

	}*/
	while (!endEmulate) {
		bool regular = cpu.DecodeAndExecute();
		if (!regular)
			cpu.SetIrregularInterrupt();
		endEmulate = cpu.HandleInterrupts();
	}

	// If he enters wrong start up instruction, fuck it.
	//
	while (endEmulate) {
		endEmulate = cpu.DecodeAndExecute(true);
	}
	// Kill threads.
	//
	//pthread_kill(CPU::timerThread, 0);
}

int Emulator::ParseRightSide(const string & curLine) {
	vTokens.clear();
	tokenizer.SplitString(curLine, vTokens, line, loaderName);
	int val = 0;

	int sign = 1;

	for (unsigned idx = 2; idx < vTokens.size(); idx++) {
		if (vTokens[idx] == "+") {
			sign = 1;
		}
		else if (vTokens[idx] == "-") {
			sign = -1;
		}
		else if (((idx + 1) < vTokens.size())) {
			if (vTokens[idx + 1] != "(") {
				if (tokenizer.isImmediate(vTokens[idx])) {
					val += sign * tokenizer.ConvertImmediateNumber(vTokens[idx]);
				}
				else {
					if ((mLoadScript.find(vTokens[idx]) == mLoadScript.end())) {
						if (mGlobal.find(vTokens[idx]) == mGlobal.end()) {
							throw GlobalSymbolNotExists(line, loaderName);
						}
						string sectionName = mGlobal.find(vTokens[idx])->second->getSection()->getName();
						if (!mSection[sectionName]->hasBeginAddress())
							throw GlobalSymbolNotExists(line, loaderName);
						val += sign * mGlobal[vTokens[idx]]->getValue() + mSection[sectionName]->getBeginAddress();
					}
					else
						val += sign * mLoadScript[vTokens[idx]]->getValue();
				}
			}
			else {
				if (vTokens[idx] != "align")
					throw WrongInstructionFormat(line, " align");
				if ((idx + 5) > vTokens.size())
					throw WrongInstructionFormat(line, " align");
				if ((vTokens[idx + 3] != ",") || (vTokens[idx + 5] != ")"))
					throw WrongInstructionFormat(line, " align");
				int address, div;

				if (tokenizer.isImmediate(vTokens[idx + 2])) {
					address = tokenizer.ConvertImmediateNumber(vTokens[idx + 2]);
				}
				else {
					if ((mLoadScript.find(vTokens[idx + 2]) == mLoadScript.end())) {
						string sectionName = mGlobal.find(vTokens[idx + 2])->second->getSection()->getName();
						if (!mSection[sectionName]->hasBeginAddress())
							throw GlobalSymbolNotExists(line, loaderName);
						address = mGlobal[vTokens[idx + 2]]->getValue() + mSection[sectionName]->getBeginAddress();
					}
					else
						address =  mLoadScript[vTokens[idx + 2]]->getValue();
				}

				if (tokenizer.isImmediate(vTokens[idx + 4])) {
					div = tokenizer.ConvertImmediateNumber(vTokens[idx + 4]);
				}
				else {
					if((mLoadScript.find(vTokens[idx + 4]) == mLoadScript.end())) {
						string sectionName = mGlobal.find(vTokens[idx + 4])->second->getSection()->getName();
						if (!mSection[sectionName]->hasBeginAddress())
							throw GlobalSymbolNotExists(line, loaderName);
						address = mGlobal[vTokens[idx + 4]]->getValue() + mSection[sectionName]->getBeginAddress();
					}
				else
					div = mLoadScript[vTokens[idx + 4]]->getValue();
				}
				val += sign * Align(address, div);

				idx += 5;
			}
		}
		else {
			if (tokenizer.isImmediate(vTokens[idx])) {
				val += sign * tokenizer.ConvertImmediateNumber(vTokens[idx]);
			}
			else {
				if ((mLoadScript.find(vTokens[idx]) == mLoadScript.end())) {
					if (mGlobal.find(vTokens[idx]) == mGlobal.end()) {
						throw GlobalSymbolNotExists(line, loaderName);
					}
					string sectionName = mGlobal.find(vTokens[idx])->second->getSection()->getName();
					if (!mSection[sectionName]->hasBeginAddress())
						throw GlobalSymbolNotExists(line, loaderName);
					val += sign * mGlobal[vTokens[idx]]->getValue() + mSection[sectionName]->getBeginAddress();
				}
				else
					val += sign * mLoadScript[vTokens[idx]]->getValue();
			}
		}

	}
	return val;
}

int Emulator::Align(int address, int div)
{
	int divide = address / div;
	if (divide * div == address)
		return address;
	return (divide + 1) * div;
}



Emulator::~Emulator()
{
}
