#pragma once

#include <map>
#include "Tokenizer.h"

using namespace std;

typedef unsigned char byte;




class Memory
{
	map <int, byte> mRam;
	map <int, byte> mIO;
	Tokenizer tokenizer;
public:
	Memory(Tokenizer _tokenizer) : tokenizer(_tokenizer) {};

	void WriteRamByte(int address, byte data);
	void WriteRamInt(int address, int data, bool little = false);

	byte ReadRamByte(int address);
	int ReadRamInt(int address, bool little = false);
	
	void WriteIOByte(int address, byte data);
	void WriteIOInt(int address, int data, bool little = false);
	
	byte ReadIOByte(int address);
	int ReadIOInt(int address, bool little = false);
	
	~Memory();
};

