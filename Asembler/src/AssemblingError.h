#pragma once

#include <cstring>
#include <string>

using namespace std;

class AssemblingError
{
	int line;
	string text;
public:

	AssemblingError(int _line, string _text = "") : line(_line), text(_text) {}
	virtual ~AssemblingError();
	string GetError() { return GetNameOfError() + " " + text; }
	virtual string GetNameOfError() { return "Base class"; }
	virtual int exitCodeReturn() { return 0; }
	int getLine() { return line; }

};


class WrongNumberOfArguments : public AssemblingError {
public:
	WrongNumberOfArguments(int line) : AssemblingError(line) {}
	string GetNameOfError() { return "Wrong number of arguments"; }
	virtual int exitCodeReturn() { return -1; }
};

class InputFileProblem : public AssemblingError {
public:
	InputFileProblem(int line) : AssemblingError(line) {}
	string GetNameOfError() { return "Input file opening problem"; }
	virtual int exitCodeReturn() { return -2; }
};

class OutputFileProblem : public AssemblingError {
public:
	OutputFileProblem(int line) : AssemblingError(line) {}
	string GetNameOfError() { return "Output file opening problem"; }
	virtual int exitCodeReturn() { return -3; }
};

class WrongInstructionFormat : public AssemblingError {
public:
	WrongInstructionFormat(int line, string text = "") : AssemblingError(line, text) {}
	string GetNameOfError() { return "Wrong instruction format"; }
	virtual int exitCodeReturn() { return -4; }
};

class LabelDoesntHaveInstruction : public AssemblingError {
public:
	LabelDoesntHaveInstruction(int line, string text = "") : AssemblingError(line, text) {}
	string GetNameOfError() { return "Label doesn't have instruction after"; }
	virtual int exitCodeReturn() { return -5; }
};

class LabelAlreadyDefined : public AssemblingError {
public:
	LabelAlreadyDefined(int line, string text = "") : AssemblingError(line, text) {}
	string GetNameOfError() { return "Label is already defined"; }
	virtual int exitCodeReturn() { return -6; }
};

class WrongNumberOfArgumentsInstruction : public AssemblingError{
public:
	WrongNumberOfArgumentsInstruction(int line, string text = "") : AssemblingError(line, text) {}
	string GetNameOfError() { return "Wrong number of arguments for instruction "; }
	virtual int exitCodeReturn() { return -7; }
};

class ImmediateValueExcpected : public AssemblingError {
public:
	ImmediateValueExcpected(int line, string text = "") : AssemblingError(line, text) {}
	string GetNameOfError() { return "Immediate value excpected "; }
	virtual int exitCodeReturn() { return -8; }
};

class TooBigImmediateValueEntered : public AssemblingError {
public:
	TooBigImmediateValueEntered(int line, string text = "") : AssemblingError(line, text) {}
	string GetNameOfError() { return "Too big or too small immediate value entered "; }
	virtual int exitCodeReturn() { return -9; }
};

class InstructionNotInTextSection : public AssemblingError {
public:
	InstructionNotInTextSection(int line, string text = "") : AssemblingError(line, text) {}
	string GetNameOfError() { return "Instruction not in text section "; }
	virtual int exitCodeReturn() { return -10; }
};

class EndDoesNotExists : public AssemblingError {
public:
	EndDoesNotExists(int line, string text = "") : AssemblingError(line, text) {}
	string GetNameOfError() { return "End doesnot exists "; }
	virtual int exitCodeReturn() { return -11; }
};

class WrongNumberOfArgumentsDirective: public AssemblingError {
public:
	WrongNumberOfArgumentsDirective(int line, string text = "") : AssemblingError(line, text) {}
	string GetNameOfError() { return "Wrong number of arguments for directive "; }
	virtual int exitCodeReturn() { return -12; }
};

class NotBinaryNumber : public AssemblingError {
public:
	NotBinaryNumber(int line, string text = "") : AssemblingError(line, text) {}
	string GetNameOfError() { return "This is not a binary number "; }
	virtual int exitCodeReturn() { return -12; }
};

class DirectiveNotInSection: public AssemblingError {
public:
	DirectiveNotInSection(int line, string text = "") : AssemblingError(line, text) {}
	string GetNameOfError() { return "Instruction not in text section "; }
	virtual int exitCodeReturn() { return -10; }
};

class DirectiveInSection : public AssemblingError {
public:
	DirectiveInSection(int line, string text = "") : AssemblingError(line, text) {}
	string GetNameOfError() { return "Directive not in text section "; }
	virtual int exitCodeReturn() { return -10; }
};


class WrongDirectiveFormat : public AssemblingError {
public:
	WrongDirectiveFormat(int line, string text = "") : AssemblingError(line, text) {}
	string GetNameOfError() { return "Wrong directive format"; }
	virtual int exitCodeReturn() { return -4; }
};