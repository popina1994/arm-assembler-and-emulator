#pragma once

#include "Memory.h"
#include "Tokenizer.h"

class IVT
{
	Memory* m;
	Tokenizer* t;
public:
	void InitTable();
	IVT(Memory* _m, Tokenizer* _t) : m(_m), t(_t) {};
	int getRoutine(int entry);
	~IVT();
};

