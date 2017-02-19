#include <iostream>
#include <unistd.h>
#include <inttypes.h>


#include "CPU.h"

bool DEBUG = false;
volatile int CPU::interuptRegister = 0;
volatile char CPU::c = -1;
pthread_mutex_t CPU::mutexInterrupt;

using namespace std;




bool CPU::DecodeAndExecute(bool secondPass)
{
	int ret;

	int instr = memory->ReadRamInt(regs[PC]);
	regs[PC] += 4;
	int cond = getBits(instr, 29, 3);

	int flagChange = getBits(instr, 28, 1);

	int instrType = getBits(instr, 24, 4);

	if (cond == 0) {
		if (!IsSetZeroFlag()) {
			ret = true;
			goto exit;
		}
	}
	else if (cond == 1) {
		if (IsSetZeroFlag()) {
			ret = true;
			goto exit;
		}
	}
	else if (cond == 2) {
		if (!(IsSetZeroFlag() && (IsSetNegativeFlag() == IsSetOverflowFlag()))) {
			ret = true;
			goto exit;
		}
	}
	else if (cond == 3) {
		if (!(IsSetNegativeFlag() == IsSetOverflowFlag())) {
			ret = true;
			goto exit;
		}
	}
	else if (cond == 4) {
		if (!(IsSetNegativeFlag() != IsSetOverflowFlag())) {
			ret = true;
			goto exit;
		}
	}
	else if (cond == 5) {
		if (!(IsSetZeroFlag() && (IsSetNegativeFlag() != IsSetOverflowFlag()))) {
			ret = true;
			goto exit;
		}
	}
	else if (cond == 6) {
		ret = false;
		goto exit;
	}

	// INT
	if (instrType == 0) {
		int srcReg = getBits(instr, 20, 4);
		if (srcReg >= NUM_NORMAL_REGS) {
			ret = false;
			goto exit;
		}


		int oldInterruptRegister = interuptRegister;
		while (!TestAndSetInterrupt(oldInterruptRegister, srcReg)) {
			oldInterruptRegister = interuptRegister;
		}
		ret = true;
		goto exit;
	}

	// ADD
	//
	else if (instrType == 1) {
		int isImmediate = getBits(instr, 18, 1);
		int dstReg = getBits(instr, 19, 5);
		int64_t result = 0;
		uint64_t resultU = 0;
		// Addition of immediate and value from reg.
		//
		if (isImmediate) {
			int immediate = ExtendImmediate(instr, 0, 18);
			result += (int64_t)regs[dstReg] + (int64_t)immediate;
			resultU += (uint64_t)((unsigned)regs[dstReg]) + (uint64_t)(unsigned)(immediate);
			regs[dstReg] = regs[dstReg] + immediate;

		}
		else {
			int srcReg = getBits(instr, 13, 5);
			result += (int64_t)regs[dstReg] + (int64_t)regs[srcReg];
			resultU += (uint64_t)(unsigned)regs[dstReg] + (uint64_t)(unsigned)regs[srcReg];
			regs[dstReg] = regs[dstReg] + regs[srcReg];

		}
		if (flagChange) {
			SetOverflowFlag(result != regs[dstReg]);
			SetCarryFlag(resultU & (1ull << 32));
			UpdateZeroFlag(regs[dstReg]);
			UpdateNegativeFlag(regs[dstReg]);
		}

		ret = true;
		goto exit;
	}


	// SUB
	//
	else if (instrType == 2) {
		int isImmediate = getBits(instr, 18, 1);
		int dstReg = getBits(instr, 19, 5);
		int64_t result = 0;
		uint64_t resultU = 0;
		// Addition of immediate and value from reg.
		//
		if (isImmediate) {
			int immediate = ExtendImmediate(instr, 0, 18);
			result += (int64_t)regs[dstReg] - (int64_t)immediate;
			resultU += (uint64_t)((unsigned)regs[dstReg]) + (uint64_t)(unsigned)(-immediate);
			regs[dstReg] = regs[dstReg] - immediate;

		}
		else {
			int srcReg = getBits(instr, 13, 5);
			result += (int64_t)regs[dstReg] - (int64_t)regs[srcReg];
			resultU += (uint64_t)(unsigned)regs[dstReg] + (uint64_t)(unsigned)(-regs[srcReg]);
			regs[dstReg] = regs[dstReg] - regs[srcReg];

		}
		if (flagChange) {
			SetOverflowFlag(result != regs[dstReg]);
			SetCarryFlag(resultU & (1ull << 32));
			UpdateZeroFlag(regs[dstReg]);
			UpdateNegativeFlag(regs[dstReg]);
		}

		ret = true;
		goto exit;
	}

	// MUL
	//
	else if (instrType == 3) {
		int isImmediate = getBits(instr, 18, 1);
		int dstReg = getBits(instr, 19, 5);
		// Addition of immediate and value from reg.
		//
		if (isImmediate) {
			int immediate = ExtendImmediate(instr, 0, 18);
			regs[dstReg] = regs[dstReg] * immediate;

		}
		else {
			int srcReg = getBits(instr, 13, 5);
			regs[dstReg] = regs[dstReg] * regs[srcReg];

		}
		if (flagChange) {
			UpdateZeroFlag(regs[dstReg]);
			UpdateNegativeFlag(regs[dstReg]);
		}

		ret = true;
		goto exit;
	}

	// DIV
	//
	else if (instrType == 4) {
		int isImmediate = getBits(instr, 18, 1);
		int dstReg = getBits(instr, 19, 5);
		// Addition of immediate and value from reg.
		//
		if (isImmediate) {
			int immediate = ExtendImmediate(instr, 0, 18);
			regs[dstReg] = regs[dstReg] / immediate;

		}
		else {
			int srcReg = getBits(instr, 13, 5);
			regs[dstReg] = regs[dstReg] / regs[srcReg];

		}
		if (flagChange) {
			UpdateZeroFlag(regs[dstReg]);
			UpdateNegativeFlag(regs[dstReg]);
		}

		ret = true;
		goto exit;
	}

	// CMP
	//
	else if (instrType == 5) {
		int isImmediate = getBits(instr, 18, 1);
		int dstReg = getBits(instr, 19, 5);
		int64_t result = 0;
		uint64_t resultU = 0;
		int dummy = regs[dstReg];
		// Addition of immediate and value from reg.
		//
		if (isImmediate) {
			int immediate = ExtendImmediate(instr, 0, 18);
			result += (int64_t)regs[dstReg] - (int64_t)immediate;
			resultU += (uint64_t)((unsigned)regs[dstReg]) + (uint64_t)(unsigned)(-immediate);
			dummy = regs[dstReg] - immediate;

		}
		else {
			int srcReg = getBits(instr, 13, 5);
			result += (int64_t)regs[dstReg] - (int64_t)regs[srcReg];
			resultU += (uint64_t)(unsigned)regs[dstReg] + (uint64_t)(unsigned)(-regs[srcReg]);
			dummy = regs[dstReg] - regs[srcReg];

		}
		if (flagChange) {
			SetOverflowFlag(result != dummy);
			SetCarryFlag(resultU & (1ull << 32));
			UpdateZeroFlag(dummy);
			UpdateNegativeFlag(dummy);
		}

		ret = true;
		goto exit;
	}

	// AND
	//
	else if (instrType == 6) {
		int dstReg = getBits(instr, 19, 5);
		int srcReg = getBits(instr, 14, 5);

		regs[dstReg] &= regs[srcReg];

		if (flagChange) {
			UpdateZeroFlag(regs[dstReg]);
			UpdateNegativeFlag(regs[dstReg]);
		}

		ret = true;
		goto exit;
	}
	// OR
	//
	else if (instrType == 7) {
		int dstReg = getBits(instr, 19, 5);
		int srcReg = getBits(instr, 14, 5);

		regs[dstReg] |= regs[srcReg];

		if (flagChange) {
			UpdateZeroFlag(regs[dstReg]);
			UpdateNegativeFlag(regs[dstReg]);
		}

		ret = true;
		goto exit;
	}
	//NOT
	//
	else if (instrType == 8) {
		int dstReg = getBits(instr, 19, 5);
		int srcReg = getBits(instr, 14, 5);

		regs[dstReg] = ~regs[srcReg];

		if (flagChange) {
			UpdateZeroFlag(regs[dstReg]);
			UpdateNegativeFlag(regs[dstReg]);
		}

		ret = true;
		goto exit;
	}
	// TEST
	//
	else if (instrType == 9) {
		int dstReg = getBits(instr, 19, 5);
		int srcReg = getBits(instr, 14, 5);

		int dummy = regs[dstReg] & regs[srcReg];

		if (flagChange) {
			UpdateZeroFlag(dummy);
			UpdateNegativeFlag(dummy);
		}

		ret = true;
		goto exit;
	}
	// LDR, STR
	//
	else if (instrType == 10) {
		int addrReg = getBits(instr, 19, 5);
		int dstReg = getBits(instr, 14, 5);

		int typeOfIncDec = getBits(instr, 11, 3);

		int load = getBits(instr, 10, 1);
		int immediate = ExtendImmediate(instr, 0, 10, true);

		if (typeOfIncDec == 4) {
			regs[addrReg] += 4;
		}
		else if (typeOfIncDec == 5) {
			regs[addrReg] -= 4;
		}

		if (load) {
			// In future if it's reading from data, it will be reordered.
			//
			regs[dstReg] = memory->ReadRamInt(regs[addrReg] + immediate, true);
		}
		else {
			memory->WriteRamInt(regs[addrReg], regs[dstReg] + immediate, true);
		}

		if (typeOfIncDec == 2) {
			regs[addrReg] += 4;
		}
		else if (typeOfIncDec == 3) {
			regs[addrReg] -= 4;
		}
		ret = true;
		goto exit;

	}

	// CALL
	else if (instrType == 12) {
		int dstReg = getBits(instr, 19, 5);
		int immediate = ExtendImmediate(instr, 0, 19, true);
		regs[LR] = regs[PC];
		regs[PC] = regs[dstReg] + immediate;
		ret = true;
		goto exit;
	}

	// IN / OUT
	if (instrType == 13) {
		int dstReg = getBits(instr, 20, 4);
		int srcReg = getBits(instr, 16, 4);
		int input = getBits(instr, 15, 1);
		if (input) {
			if (regs[srcReg] == CONSOLE_READ_ADDRESS) {
				regs[dstReg] = buffer.front();
			buffer.pop_front();
				if (buffer.empty()) {
					int keyReg = memory->ReadIOInt(KEY_PRESSED);
					keyReg &= ~(MASK_KEY_PRESSED);
					memory->WriteIOInt(KEY_PRESSED, keyReg);
				}
			}
			else {

				regs[dstReg] = memory->ReadIOInt(regs[srcReg]);
			}
		}
		else {
			if (regs[srcReg] == CONSOLE_WRITE_ADRESS) {
				cout << (char)regs[dstReg];
				cout.flush();
			}
			else {
				memory->WriteIOInt(regs[srcReg], regs[dstReg]);
			}
		}
		ret = true;
		goto exit;
	}

	// MOV, SHR, SHL
	//
	else if (instrType == 14) {
		int dstReg = getBits(instr, 19, 5);
		if (dstReg >= NUM_REGS) {
			ret = false;
			goto exit;
		}
		int srcReg = getBits(instr, 14, 5);
		if (srcReg >= NUM_REGS) {
			ret = false;
			goto exit;
		}

		int immediate = ExtendImmediate(instr, 9, 5, false);
		int left = getBits(instr, 8, 1);
		if (flagChange && immediate == 0) {
			SetCarryFlag(0);
		}

		if (left) {
			if ( (immediate != 0) && flagChange) {
				SetCarryFlag( regs[dstReg] & (1 << (32 - immediate)));
			}
			regs[dstReg] = (unsigned)(regs[srcReg]) << ((unsigned)(immediate));
		}
		else {
			if ( (immediate != 0) && flagChange) {
				SetCarryFlag(regs[dstReg] & (1 << (immediate -1)));
			}
			regs[dstReg] = (unsigned)(regs[srcReg]) >> ((unsigned)(immediate));
		}


		if (flagChange) {
			UpdateZeroFlag(regs[dstReg]);
			UpdateNegativeFlag(regs[dstReg]);
		}

		// iret
		//
		if (flagChange && (dstReg == PC)) {
			if (secondPass) {
				ret = false;
				goto exit;
			}
			else {
				InterruptExit();
			}

		}
		ret = true;
		goto exit;

	}
	// LDCAL
	//
	else if (instrType == 15) {
		int dstReg = getBits(instr, 20, 4);
		int isHigh = getBits(instr, 19, 1);
		int immediate = getBits(instr, 0, 16);
		if (isHigh) {

			regs[dstReg] |= (immediate << 16) ;
		}
		else {
			regs[dstReg] = immediate;
		}

		ret = true;
		goto exit;
	}
	else {
		ret = false;
		goto exit;
	}


exit:
	if (DEBUG) {
		cout << instrType << " " << hex << regs[PSW] << endl;
	}
	return ret;
}

void CPU::SetIrregularInterrupt()
{
	int oldInterruptRegister = interuptRegister;
	while (!TestAndSetInterrupt(oldInterruptRegister, MASK_IRREGULAR_INSTR_INTERRUPT)) {
		oldInterruptRegister = interuptRegister;
	}
}

void CPU::SetGlobalInterrupt(bool turnOn)
{
	if (turnOn)
		regs[PSW] |= MASK_GLOBAL;
	else
		regs[PSW] &= ~MASK_GLOBAL;
}

void CPU::
SetZeroFlag(bool turnOn)
{
	if (turnOn) {
		regs[PSW] |= MASK_ZERO;
	}
	else {
		regs[PSW] &= ~MASK_ZERO;
	}
}

void CPU::SetCarryFlag(bool turnOn)
{
	if (turnOn) {
		regs[PSW] |= MASK_CARY;
	}
	else {
		regs[PSW] &= ~MASK_CARY;
	}
}

void CPU::SetNegativeFlag(bool turnOn)
{
	if (turnOn) {
		regs[PSW] |= MASK_NEGATIVE;
	}
	else {
		regs[PSW] &= ~MASK_NEGATIVE;
	}
}

void CPU::SetOverflowFlag(bool turnOn)
{
	if (turnOn) {
		regs[PSW] |= MASK_OVERFLOW;
	}
	else {
		regs[PSW] &= ~MASK_OVERFLOW;
	}
}

void CPU::setTimerFlag(bool turnOn)
{

	if (turnOn) {
		regs[PSW] |= MASK_TIMER;
	}
	else {
		regs[PSW] &= ~MASK_TIMER;
	}
}

void CPU::UpdateZeroFlag(int result)
{
	SetZeroFlag(result == 0);
}



void CPU::UpdateNegativeFlag(int result)
{
	SetNegativeFlag(result < 0);
}

int CPU::ExtendImmediate(int num, int pos, int len, bool sign)
{
	int n = getBits(num, pos, len);
	if ((n &   (1 << (len - 1))) && sign) {
		int mask =  ~((1 << len) - 1);
			return n | mask;
	}
	return n;
}

bool CPU::IsSetGlobalInterrupt()
{
	return regs[PSW] & MASK_GLOBAL;
}

bool CPU::IsSetZeroFlag()
{
	return regs[PSW] & MASK_ZERO;
}

bool CPU::IsSetCarryFlag()
{
	return regs[PSW] & MASK_CARY;
}

bool CPU::IsSetNegativeFlag()
{
	return regs[PSW] & MASK_NEGATIVE;
}

bool CPU::IsSetOverflowFlag()
{
	return regs[PSW] & MASK_OVERFLOW;
}

bool CPU::IsSetTimerFlag()
{
	return regs[PSW] & MASK_TIMER;
}
// pos denotes index of first number from right to left of bits
// which are going to be exctracted.
//

int CPU::getBits(int num, int pos, int len)
{
	return ((num & ((1 << len) - 1) << pos) >> pos) &  (( 1 << len) - 1);
}






bool CPU::TestAndSetInterrupt(int oldInterruptRegister, int bit, int turnOn)
{
	pthread_mutex_lock(&mutexInterrupt);
	if (oldInterruptRegister != interuptRegister) {
		pthread_mutex_unlock(&mutexInterrupt);
		return false;
	}
	else {
		if (turnOn)
			interuptRegister |= 1 << bit;
		else
			interuptRegister &= ~(1 << bit);
		pthread_mutex_unlock(&mutexInterrupt);
		return true;
	}
}

void CPU::InterruptEnter(int routine) {
	SetGlobalInterrupt(true);
	Push(regs[LR]);
	Push(regs[PSW]);
	regs[LR] = regs[PC];
	regs[PC] = ivt->getRoutine(routine);
}

void CPU::InterruptExit()
{
	//regs[PC] = regs[LR];
	regs[PSW] = Pop();
	regs[LR] = Pop();
	SetGlobalInterrupt(false);

}



bool CPU::HandleInterrupts()
{
	static int cnt = 0;
	// Case when is inside of interrupt, another interupt cannot be handled.
	//
	if (IsSetGlobalInterrupt())
		return false;
	int oldInterruptRegister = interuptRegister;
	for (int idx = 0; idx < 16; idx++) {
		if ((oldInterruptRegister & (1 << idx))) {

			if (idx == 3) {
				char localC = c;
				// In future resolve this.
				//
				buffer.push_back(localC);
				int keyReg = memory->ReadIOInt(KEY_PRESSED);
				keyReg |= MASK_KEY_PRESSED;
				memory->WriteIOInt(KEY_PRESSED, keyReg);
			}
			// Clear the interrupt.
			//
			while (!TestAndSetInterrupt(oldInterruptRegister, idx, false)) {
				oldInterruptRegister = interuptRegister;
				}
			// End programm.
			//
			if (idx == 0) {
				cout << endl << "CPU is shuting down" << endl;
				InterruptEnter(idx);
				return true;
			}
			// Periodic interrupt.
			//
			if ((idx == 1) && (IsSetTimerFlag())) {
				cout << "Usao u timer" << cnt ++ << endl;
				InterruptEnter(idx);
				break;
			}
			// Incorrect instruction, keyboard flag.
			//
			else if (idx == 2) {
				cout << "Nekorektna instrukcija" << endl;
			}
			else {
				InterruptEnter(idx);
				break;
			}
		}
	}
	return false;

}

void CPU::Push(int data)
{
	regs[SP] -= 4;
	memory->WriteRamInt(regs[SP], data, true);
}

int CPU::Pop()
{
	int data = memory->ReadRamInt(regs[SP], true);
	regs[SP] += 4;
	return data;
}


void * CPU::KeyboardHandler(void *)
{

	while (true) {
		char localC;
		cin >> localC;
		c = localC;
		int oldInterruptRegister = interuptRegister;
		// Write character.
		//
		while (!TestAndSetInterrupt(oldInterruptRegister, MASK_KEYBOARD_INTERRUPT));
		// Wait to character be stored in buffer.
		//
		while (interuptRegister & (1 << MASK_KEYBOARD_INTERRUPT));

	}

	return NULL;
}

void * CPU::Timer(void *)
{
	//static int cnt = 0;
	while (true) {
		int oldInterruptRegister = interuptRegister;
		// Write character.
		//

		while (!TestAndSetInterrupt(oldInterruptRegister, MASK_TIMER_INTERRUPT));
		sleep(1);
		//cout << endl << cnt++ << endl;
	}
	return NULL;
}

CPU::~CPU()
{
}
