#pragma once

#include <cstring>
#include <string>

using namespace std;

class EmulatorError
{
	int line;
	string text;
public:

	EmulatorError(int _line, string _text = "") : line(_line), text(_text) {}
	string GetError() { return GetNameOfError() + " " + text; }
	virtual string GetNameOfError() { return "Base class"; }
	virtual int exitCodeReturn() { return 0; }
	int getLine() { return line; }

};


class WrongNumberOfArguments : public EmulatorError {
public:
	WrongNumberOfArguments(int line) : EmulatorError(line) {}
	string GetNameOfError() { return "Wrong number of arguments"; }
	virtual int exitCodeReturn() { return -1; }
};

class InputFileProblem : public EmulatorError {
public:
	InputFileProblem(int line) : EmulatorError(line) {}
	string GetNameOfError() { return "Input file opening problem"; }
	virtual int exitCodeReturn() { return -2; }
};

class OutputFileProblem : public EmulatorError {
public:
	OutputFileProblem(int line) : EmulatorError(line) {}
	string GetNameOfError() { return "Output file opening problem"; }
	virtual int exitCodeReturn() { return -3; }
};

class WrongInstructionFormat : public EmulatorError {
public:
	WrongInstructionFormat(int line, string text = "") : EmulatorError(line, text) {}
	string GetNameOfError() { return "Wrong instruction format"; }
	virtual int exitCodeReturn() { return -4; }
};

class LabelDoesntHaveInstruction : public EmulatorError {
public:
	LabelDoesntHaveInstruction(int line, string text = "") : EmulatorError(line, text) {}
	string GetNameOfError() { return "Label doesn't have instruction after"; }
	virtual int exitCodeReturn() { return -5; }
};

class LabelAlreadyDefined : public EmulatorError {
public:
	LabelAlreadyDefined(int line, string text = "") : EmulatorError(line, text) {}
	string GetNameOfError() { return "Label is already defined"; }
	virtual int exitCodeReturn() { return -6; }
};

class WrongNumberOfArgumentsInstruction : public EmulatorError{
public:
	WrongNumberOfArgumentsInstruction(int line, string text = "") : EmulatorError(line, text) {}
	string GetNameOfError() { return "Wrong number of arguments for instruction "; }
	virtual int exitCodeReturn() { return -7; }
};

class ImmediateValueExcpected : public EmulatorError {
public:
	ImmediateValueExcpected(int line, string text = "") : EmulatorError(line, text) {}
	string GetNameOfError() { return "Immediate value excpected "; }
	virtual int exitCodeReturn() { return -8; }
};

class TooBigImmediateValueEntered : public EmulatorError {
public:
	TooBigImmediateValueEntered(int line, string text = "") : EmulatorError(line, text) {}
	string GetNameOfError() { return "Too big or too small immediate value entered "; }
	virtual int exitCodeReturn() { return -9; }
};

class InstructionNotInTextSection : public EmulatorError {
public:
	InstructionNotInTextSection(int line, string text = "") : EmulatorError(line, text) {}
	string GetNameOfError() { return "Instruction not in text section "; }
	virtual int exitCodeReturn() { return -10; }
};

class EndDoesNotExists : public EmulatorError {
public:
	EndDoesNotExists(int line, string text = "") : EmulatorError(line, text) {}
	string GetNameOfError() { return "End doesnot exists "; }
	virtual int exitCodeReturn() { return -11; }
};

class WrongNumberOfArgumentsDirective: public EmulatorError {
public:
	WrongNumberOfArgumentsDirective(int line, string text = "") : EmulatorError(line, text) {}
	string GetNameOfError() { return "Wrong number of arguments for directive "; }
	virtual int exitCodeReturn() { return -12; }
};

class NotBinaryNumber : public EmulatorError {
public:
	NotBinaryNumber(int line, string text = "") : EmulatorError(line, text) {}
	string GetNameOfError() { return "This is not a binary number "; }
	virtual int exitCodeReturn() { return -13; }
};

class DirectiveNotInSection: public EmulatorError {
public:
	DirectiveNotInSection(int line, string text = "") : EmulatorError(line, text) {}
	string GetNameOfError() { return "Instruction not in text section "; }
	virtual int exitCodeReturn() { return -14; }
};

class DirectiveInSection : public EmulatorError {
public:
	DirectiveInSection(int line, string text = "") : EmulatorError(line, text) {}
	string GetNameOfError() { return "Directive not in text section "; }
	virtual int exitCodeReturn() { return -15; }
};


class WrongDirectiveFormat : public EmulatorError {
public:
	WrongDirectiveFormat(int line, string text = "") : EmulatorError(line, text) {}
	string GetNameOfError() { return "Wrong directive format"; }
	virtual int exitCodeReturn() { return -16; }
};


class ChangedObjFile : public EmulatorError {
public:
	ChangedObjFile(int line, string text = "") : EmulatorError(line, text) {}
	string GetNameOfError() { return "Object file is changed"; }
	virtual int exitCodeReturn() { return -17; }
};

class DuplicateGlobalSymbol : public EmulatorError {
public:
	DuplicateGlobalSymbol(int line, string text = "") : EmulatorError(line, text) {}
	string GetNameOfError() { return "Duplicate global symbol"; }
	virtual int exitCodeReturn() { return -18; }
};

class DuplicateSectionSymbol : public EmulatorError {
public:
	DuplicateSectionSymbol(int line, string text = "") : EmulatorError(line, text) {}
	string GetNameOfError() { return "Section name already defined"; }
	virtual int exitCodeReturn() { return -19; }
};

class SectionDoesNotExists : public EmulatorError {
public:
	SectionDoesNotExists(int line, string text = "") : EmulatorError(line, text) {}
	string GetNameOfError() { return "Section doesn't exist defined"; }
	virtual int exitCodeReturn() { return -20; }
};


class AddressLess : public EmulatorError {
public:
	AddressLess(int line, string text = "") : EmulatorError(line, text) {}
	string GetNameOfError() { return "Not alowwed address"; }
	virtual int exitCodeReturn() { return -21; }
};

class GlobalSymbolNotExists: public EmulatorError {
public:
	GlobalSymbolNotExists(int line, string text = "") : EmulatorError(line, text) {}
	string GetNameOfError() { return "Global symbol not exists"; }
	virtual int exitCodeReturn() { return -22; }
};


class UnresolvedSymbol : public EmulatorError {
public:
	UnresolvedSymbol(int line, string text = "") : EmulatorError(line, text) {}
	string GetNameOfError() { return "Extern symbol does not exist"; }
	virtual int exitCodeReturn() { return -23; }
};


class MainDoesNotExist : public EmulatorError {
public:
	MainDoesNotExist(int line, string text = "") : EmulatorError(line, text) {}
	string GetNameOfError() { return "Main doesn't exist"; }
	virtual int exitCodeReturn() { return -24; }
};

class BeginAddressAlreadySet : public EmulatorError {
public:
	BeginAddressAlreadySet(int line, string text = "") : EmulatorError(line, text) {}
	string GetNameOfError() { return "Begin address already set"; }
	virtual int exitCodeReturn() { return -24; }
};