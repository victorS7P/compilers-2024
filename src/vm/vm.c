#ifndef VM
#define VM

#include "../utils/utils.c"
#include "../config/config.c"

#include "./vm.h"

TNeanderVM createNeanderVM() {
	TNeanderVM vm;

	vm.accumulator = 0;
	vm.programCounter = 0;
	vm.accessesCounter = 0;
	vm.instructionsCounter = 0;
	updateFlags(&vm);

	return vm;
}

void debugVmMemory(TNeanderVM *vm) {
	for (int i = 0; i < MEMORY_SIZE; i++)
		debug("Memory[%03d]: %03d\n", i, vm->memory[i]);
}

void debugVm(TNeanderVM *vm) {
	debug("Accumulator: %03d\n", vm->accumulator);
	debug("vm Counter: %03d\n", vm->programCounter);

	debug("Flag N: %d\n", vm->flagN);
	debug("Flag Z: %d\n", vm->flagZ);

	debug("Access Count: %d\n", vm->accessesCounter);
	debug("Instructions: %d\n\n", vm->instructionsCounter);
}

void updateFlagN(TNeanderVM* vm) {
	if (vm->accumulator >= 127)
		vm->flagN = 1;
	else
		vm->flagN = 0;
}

void updateFlagZ(TNeanderVM* vm) {
	if (vm->accumulator == 0)
		vm->flagZ = 1;
	else
		vm->flagZ = 0;
}

void updateFlags(TNeanderVM* vm) {
	updateFlagN(vm);
	updateFlagZ(vm);
}

int normalizeValue(int value) {
	if (value < 0) return (value + 256);
	else if (value > 255) return (value - 256);
	else return value;
}

void normalizeAccumulator(TNeanderVM* vm) {
	vm->accumulator = normalizeValue(vm->accumulator);
}

void setMemoryValue(TNeanderVM* vm, int address, int value) {
	vm->accessesCounter++;
	vm->memory[address] = normalizeValue(value);
}

int getMemoryValue(TNeanderVM* vm, int address) {
	vm->accessesCounter++;
	return vm->memory[address];
}

int getCurrentMemoryValue(TNeanderVM* vm) {
	return getMemoryValue(vm, vm->programCounter);
}

int getNextMemoryValue(TNeanderVM* vm) {
	return getMemoryValue(vm, vm->programCounter + 1);
}

int nextAsAddressToValue(TNeanderVM* vm) {
	int value = getMemoryValue(vm, getNextMemoryValue(vm));
	value = normalizeValue(value);
	
	skipNextMemorySlot(vm);
	return value;
}

void skipNextMemorySlot(TNeanderVM* vm) {
	incrementProgramCounter(vm);
}

void incrementProgramCounter(TNeanderVM* vm) {
	vm->programCounter += 1;
}

void incrementInstructionsCounter(TNeanderVM* vm) {
	vm->instructionsCounter += 1;
}

void jumpAndDecrementProgramCounter(TNeanderVM* vm) {
	vm->programCounter = getNextMemoryValue(vm);
	vm->programCounter--;
}

int setValueInMemoryFromLine(TNeanderVM *vm, int* memoryIndex, char line[4]) {
	int data = 0;
	sscanf(line, "%d", &data);

	if (data > 255) {
		debug("Invalid data at line %d\nError: data must be less than 256\n", memoryIndex + 1);
		return -1;
	}

	vm->memory[(*memoryIndex)++] = data;
}

int setNeanderVMFromStringBuffer(TNeanderVM *vm, char* buffer) {
	int  bufferLen = strlen(buffer);

	int  memoryIndex = 0;
	int  currentCharIndex = 0;
	char currentChar = 0;

	char line[4];
	int  lineIndex = 0;

	while (currentCharIndex <= bufferLen) {	
		currentChar = buffer[currentCharIndex];

		if (currentChar == '\n') {
			if (setValueInMemoryFromLine(vm, &memoryIndex, line) == -1)
				return -1;

			memset(line, '\0', sizeof(line));
			lineIndex = 0;
		} else {
			line[lineIndex] = currentChar;
			lineIndex++;
		}

		currentCharIndex++;
	}

	setValueInMemoryFromLine(vm, &memoryIndex, line);
	while (memoryIndex < MEMORY_SIZE) {
		vm->memory[memoryIndex] = 0;
		memoryIndex++;
	}

	return 0;
}

int executeVM(TNeanderVM* vm) {
	int currentOperation = getCurrentMemoryValue(vm);

	while (currentOperation != HLT) {
		if (currentOperation == STA) {
			debug("STA operation");
			incrementInstructionsCounter(vm);

			int address = getNextMemoryValue(vm);
			setMemoryValue(vm, address, vm->accumulator);
			skipNextMemorySlot(vm);
		}

		if (currentOperation == LDA) {
			debug("LDA operation");
			incrementInstructionsCounter(vm);
			vm->accumulator = nextAsAddressToValue(vm);
		}

		if (currentOperation == ADD) {
			debug("ADD operation");
			incrementInstructionsCounter(vm);
			vm->accumulator += nextAsAddressToValue(vm);
		}

		if (currentOperation == OR) {
			debug("OR operation");
			incrementInstructionsCounter(vm);
			vm->accumulator |= nextAsAddressToValue(vm);
		}

		if (currentOperation == AND) {
			debug("AND operation");
			incrementInstructionsCounter(vm);
			vm->accumulator &= nextAsAddressToValue(vm);
		}

		if (currentOperation == NOT) {
			debug("NOT operation");
			incrementInstructionsCounter(vm);
			vm->accumulator = ~vm->accumulator;
		}

		if (currentOperation == JMP) {
			debug("JMP operation");
			incrementInstructionsCounter(vm);
			jumpAndDecrementProgramCounter(vm);
		}

		if (currentOperation == JN) {
			debug("JN operation");
			incrementInstructionsCounter(vm);

			if (vm->flagN == 1)
				jumpAndDecrementProgramCounter(vm);
			else
				skipNextMemorySlot(vm);
		}

		if (currentOperation == JZ) {
			debug("JZ operation");
			incrementInstructionsCounter(vm);

			if (vm->flagZ == 1)
				jumpAndDecrementProgramCounter(vm);
			else
				skipNextMemorySlot(vm);
		}

		if (vm->programCounter >= MEMORY_SIZE) {
			debug("Program Counter out of bounds!");
			return -1;
		}

		normalizeAccumulator(vm);
		updateFlags(vm);
		incrementProgramCounter(vm);
		currentOperation = getCurrentMemoryValue(vm);
	}

	incrementInstructionsCounter(vm);
	incrementProgramCounter(vm);

	debug("Program executed successfully!\n\n");
	debugVmMemory(vm);
	debug("\n");
	debugVm(vm);
	debug("\n");

	return 0;
}

#endif