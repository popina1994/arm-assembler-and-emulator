#include "IVT.h"



void IVT::InitTable()
{
	for (int idx = 0; idx < 16; idx++) {
		// iret in future needed.
		//
		m->WriteRamInt((48 + idx) * 32, 0xfe844000);
		// Adress.
		//
		m->WriteRamInt(idx * 4, (48 + idx) * 32);
	}
}


int IVT::getRoutine(int entry)
{
	return m->ReadRamInt(entry * 4);
}

IVT::~IVT()
{
}
