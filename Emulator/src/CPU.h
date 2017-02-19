#pragma once

#include <cstdint>
#include "Memory.h"
#include "IVT.h"
#define HAVE_STRUCT_TIMESPEC
#include <pthread.h>
#include <deque>

using namespace std;



class CPU
{
	// Koji prekid je signaliziran.
	//
	volatile static int interuptRegister;
	volatile static char c;
	Memory* memory;
	IVT* ivt;
	Tokenizer* tokenizer;
	// Buffer for input.
	//
	deque <char> buffer;
public:
	int regs[20];
	static const int PC = 16;
	static const int LR = 17;
	static const int SP = 18;
	static const int PSW = 19;
	static const int NUM_NORMAL_REGS = 16;
	static const int NUM_REGS = 20;
	static const int MASK_NEGATIVE = 0x8;
	static const int MASK_OVERFLOW = 0x4;
	static const int MASK_ZERO = 0x2;
	static const int MASK_CARY = 0x1;
	static const int CONSOLE_READ_ADDRESS = 0x1000;
	static const int CONSOLE_WRITE_ADRESS = 0x2000;
	static const int KEY_PRESSED = 0x1010;
	static const int MASK_KEY_PRESSED = 1 << 10;
	
	static const int MASK_GLOBAL = 1 << 31;
	static const int MASK_TIMER = 1 << 30;

	static const int MASK_KEYBOARD_INTERRUPT = 3;
	static const int MASK_IRREGULAR_INSTR_INTERRUPT = 2;
	static const int MASK_TIMER_INTERRUPT = 1;

	static pthread_mutex_t mutexInterrupt;

	static void* KeyboardHandler(void *);

	static void* Timer(void *);

	// Returns true if instruction is correct.
	// endEmulate is only used for second pass, when it comes to rti, it will shutdown processor.
	//
	bool DecodeAndExecute(bool secondPass = false);

	void SetIrregularInterrupt();

	void SetGlobalInterrupt(bool turnOn);
	void SetZeroFlag(bool turnOn);
	void SetCarryFlag(bool turnOn);
	void SetNegativeFlag(bool turnOn);
	void SetOverflowFlag(bool turnOn);
	void setTimerFlag(bool turnOn);

	void UpdateZeroFlag(int result);
	void UpdateNegativeFlag(int result);
	int ExtendImmediate(int num, int pos, int len, bool sign = true);

	bool IsSetGlobalInterrupt();
	bool IsSetZeroFlag();
	bool IsSetCarryFlag();
	bool IsSetNegativeFlag();
	bool IsSetOverflowFlag();
	bool IsSetTimerFlag();
	int getBits(int num, int pos, int len);

	bool TestEqual();

	bool static TestAndSetInterrupt(int oldInterruptRegister, int bit, int turnOn = true);

	void InterruptEnter(int routine);
	void InterruptExit();


	bool HandleInterrupts();

	void Push(int data);
	int Pop();

	CPU(Memory* _m, IVT* _ivt, Tokenizer* _t) : memory(_m), ivt(_ivt), tokenizer(_t) {};
	~CPU();
};

