#include "Memory.h"




void Memory::WriteRamByte(int address, byte data)
{
	mRam[address] = data;
}

void Memory::WriteRamInt(int address, int data, bool little)
{
	if (little) {
		for (int idx = 0; idx < 4; idx++)
			mRam[address + idx] = (data &  (((1 << 8) - 1) << (idx * 8))) >> (idx * 8);
	}
	else {
		for (int idx = 0; idx < 4; idx++)
			mRam[address + idx] = (data &  (((1 << 8) - 1) << ( (3 - idx) * 8))) >> ( (3-idx) * 8);
	}
	
}

byte Memory::ReadRamByte(int address)
{
	return mRam[address];
}

int Memory::ReadRamInt(int address, bool little)
{
	int val = 0;
	if (little) {
		for (int idx = 0; idx < 4; idx++) {
			val |= mRam[address + idx] << (idx * 8);
		}
		
	}
	else {
		for (int idx = 0; idx < 4; idx++) {
			val |= mRam[address + idx] << ( (3 -idx) * 8);
		}

	}
	return val;
}

void Memory::WriteIOByte(int address, byte data)
{
	mIO[address] = data;
}

void Memory::WriteIOInt(int address, int data, bool little)
{
	if (little) {
		for (int idx = 0; idx < 4; idx++)
			mIO[address + idx] = (data &  (((1 << 8) - 1) << (idx * 8))) >> (idx * 8);
	}
	else {
		for (int idx = 0; idx < 4; idx++)
			mIO[address + idx] = (data &  (((1 << 8) - 1) << ((3 - idx) * 8))) >> ((3 - idx) * 8);
	}

}

byte Memory::ReadIOByte(int address)
{
	return mIO[address];
}

int Memory::ReadIOInt(int address, bool little)
{
	int val = 0;
	if (little) {
		for (int idx = 0; idx < 4; idx++) {
			val |= mIO[address + idx] << (idx * 8);
		}

	}
	else {
		for (int idx = 0; idx < 4; idx++) {
			val |= mIO[address + idx] << ((3 - idx) * 8);
		}

	}
	return val;
}


Memory::~Memory()
{
}
