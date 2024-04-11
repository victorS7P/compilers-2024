#ifndef VM
#define VM

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void debugNeanderMemory(TNeanderVM *vm) {
	for (int i = 0; i < MEMORY_SIZE; i++)
		printf("Memory[%03d]: %03d\n", i, vm->memory[i]);
}

void debugNeanderVM(TNeanderVM *vm) {
	printf("Accumulator: %03d\n", vm->accumulator);
	printf("vm Counter: %03d\n", vm->programCounter);

	printf("Flag N: %d\n", vm->flagN);
	printf("Flag Z: %d\n", vm->flagZ);

	printf("Access Count: %d\n", vm->accessesCounter);
	printf("Instructions: %d\n\n", vm->instructionsCounter);
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
	incremenTNeanderVMCounter(vm);
}

void incremenTNeanderVMCounter(TNeanderVM* vm) {
	vm->programCounter += 1;
}

void incrementInstructionsCounter(TNeanderVM* vm) {
	vm->instructionsCounter += 1;
}

void jumpAndDecremenTNeanderVMCounter(TNeanderVM* vm) {
	vm->programCounter = getNextMemoryValue(vm);
	vm->programCounter--;
}

#endif