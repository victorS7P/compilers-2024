#ifndef VM_H
#define VM_H

typedef enum NeanderToken {
	NOP = 0,
	STA = 16,
	LDA = 32,
	ADD = 48,
	OR 	= 64,
	AND = 80,
	NOT = 96,
	JMP = 128,
	JN	= 144,
	JZ	= 160,
	HLT = 240
} TNeanderToken;

typedef struct NeanderVM {
	int accumulator;
	int programCounter;
	int accessesCounter;
	int instructionsCounter;
	int memory[MEMORY_SIZE];
	int flagN;
	int flagZ;
} TNeanderVM;

void debugVm(TNeanderVM *program);
void debugVmMemory(TNeanderVM *program);

TNeanderVM createNeanderVM();
int normalizeValue(int value);
void normalizeAccumulator(TNeanderVM* vm);

void updateFlagN(TNeanderVM* vm);
void updateFlagZ(TNeanderVM* vm);
void updateFlags(TNeanderVM* vm);

void setMemoryValue(TNeanderVM* vm, int address, int value);
int getMemoryValue(TNeanderVM* vm, int address);
int getCurrentMemoryValue(TNeanderVM* vm);
int getNextMemoryValue(TNeanderVM* vm);
int nextAsAddressToValue(TNeanderVM* vm);

void skipNextMemorySlot(TNeanderVM* vm);
void incrementProgramCounter(TNeanderVM* vm);
void incrementInstructionsCounter(TNeanderVM* vm);
void jumpAndDecrementProgramCounter(TNeanderVM* vm);

#endif
