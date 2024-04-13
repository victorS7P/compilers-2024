#ifndef ASSEMBLER
#define ASSEMBLER

#include "./assembler.h"
#include "../vm/vm.h";
#include "../../utils/utils.c"

int compareStringOrKeepCurrentCharIndex(char* buffer, int* currentCharIndex, char* string) {
  int startCharIndex = (*currentCharIndex);

  int len = strlen(string);
  for (int i = 0; i <= len; i++) {
    if (i == len)
      return 0;
  
    if (buffer[(*currentCharIndex)++] != string[i]) {
      (*currentCharIndex) = startCharIndex;
      break;
    }
  }
    
  return -1;  
}

int getNextBlockDefinition(char* buffer, int* currentCharIndex) {
  skipBlankSymbols(buffer, currentCharIndex);
	
  if (compareStringOrKeepCurrentCharIndex(buffer, currentCharIndex, "USE") == 0)
    return USE;

  if (compareStringOrKeepCurrentCharIndex(buffer, currentCharIndex, "START") == 0)
    return START;

	if (compareStringOrKeepCurrentCharIndex(buffer, currentCharIndex, "DEFINE") == 0)
    return DEFINE;

	return -1;
}

int getNext3DigitsNumber(char* buffer, int* currentCharIndex) {
  skipBlankSymbols(buffer, currentCharIndex);

	char valueStr[3] = {
		validateNumberChar(buffer[(*currentCharIndex)++]),
		validateNumberChar(buffer[(*currentCharIndex)++]),
		validateNumberChar(buffer[(*currentCharIndex)++])
	};

	return convertValueStrToNumber(valueStr);
}

int getNextOperaion(char* buffer, int* currentCharIndex) {
	skipBlankSymbols(buffer, currentCharIndex);

  if (compareStringOrKeepCurrentCharIndex(buffer, currentCharIndex, "OR") == 0)
    return OR;

  if (compareStringOrKeepCurrentCharIndex(buffer, currentCharIndex, "JN") == 0)
    return JN;

  if (compareStringOrKeepCurrentCharIndex(buffer, currentCharIndex, "JZ") == 0)
    return JZ;

  if (compareStringOrKeepCurrentCharIndex(buffer, currentCharIndex, "NOP") == 0)
    return NOP;

  if (compareStringOrKeepCurrentCharIndex(buffer, currentCharIndex, "STA") == 0)
    return STA;

  if (compareStringOrKeepCurrentCharIndex(buffer, currentCharIndex, "LDA") == 0)
    return LDA;

  if (compareStringOrKeepCurrentCharIndex(buffer, currentCharIndex, "ADD") == 0)
    return ADD;

  if (compareStringOrKeepCurrentCharIndex(buffer, currentCharIndex, "AND") == 0)
    return AND;

  if (compareStringOrKeepCurrentCharIndex(buffer, currentCharIndex, "NOT") == 0)
    return NOT;

  if (compareStringOrKeepCurrentCharIndex(buffer, currentCharIndex, "JMP") == 0)
    return JMP;

  if (compareStringOrKeepCurrentCharIndex(buffer, currentCharIndex, "HLT") == 0)
    return HLT;

  printf("\nERROR!\nInvalid Definition!\nError: not a valid operation at %d.\n", (*currentCharIndex));
  exit(1);
}

void setMemoryData(int* memory, int address, int value) {
	if (memory[address] != 0)
		printf("\nWARNING!\nMemory address %03d is already defined.\nOverwriting it with %03d\n\n", address, value);

	debug("Setting memory [%03d] = %03d\n", address, value);
	memory[address] = value;
}

void setNextMemoryData(int* memory, char* buffer, int* currentCharIndex) {
  skipBlankSymbols(buffer, currentCharIndex);

	int memorySlot = getNext3DigitsNumber(buffer, currentCharIndex);

	if (buffer[(*currentCharIndex)++] != ' ') {
		printf("ERROR!\nInvalid Memory Definition!\nError: Must have a space between memory slot and value.\n");
		exit(1);
	}

	int value = getNext3DigitsNumber(buffer, currentCharIndex);

  setMemoryData(memory, memorySlot, value);
}

int defineFunction(int* memory, char* buffer, int* currentCharIndex, int startMemoryAddress) {
	int nextBlock = -1;
  int currentMemoryAddress = startMemoryAddress;

	while ((nextBlock = getNextBlockDefinition(buffer, currentCharIndex)) == -1) {
		int operation = getNextOperaion(buffer, currentCharIndex);
		setMemoryData(memory, currentMemoryAddress++, operation);
		
		if (operation != NOP && operation != NOT && operation != HLT) {
			int param = getNext3DigitsNumber(buffer, currentCharIndex);
			setMemoryData(memory, currentMemoryAddress++, param);
		}
	}

	return nextBlock;
}

void fetchMemoryFromBuffer(int* memory, char* buffer) {
	int bufferLen = strlen(buffer);
	int currentCharIndex = 0;

	skipBlankSymbols(buffer, &currentCharIndex);
	if (getNextBlockDefinition(buffer, &currentCharIndex) == USE) {
		debug("Processing USE block\n");

		skipBlankSymbols(buffer, &currentCharIndex);
		while (isdigit(buffer[currentCharIndex])) {
			setNextMemoryData(memory, buffer, &currentCharIndex);
			skipBlankSymbols(buffer, &currentCharIndex);
		}

		debug("\n");
	}

	int nextBlock = getNextBlockDefinition(buffer, &currentCharIndex);
	while (nextBlock == DEFINE) {
		skipBlankSymbols(buffer, &currentCharIndex);

		int startAddress = getNext3DigitsNumber(buffer, &currentCharIndex);
		debug("Definig function at address %03d\n", startAddress);

		nextBlock = defineFunction(memory, buffer, &currentCharIndex, startAddress);
		debug("\n");
	}

	if (nextBlock == START) {
		debug("Processing START block\n");

		int currentAddress = 0;
		while (currentCharIndex < bufferLen) {
			int operation = getNextOperaion(buffer, &currentCharIndex);

			setMemoryData(memory, currentAddress++, operation);

			if (operation != NOP && operation != NOT && operation != HLT) {
				int param = getNext3DigitsNumber(buffer, &currentCharIndex);
				setMemoryData(memory, currentAddress++, param);
			}

			skipBlankSymbols(buffer, &currentCharIndex);
		}

		debug("\n");
	}
}

char* saveMemoryToBuffer(int* memory) {
	char* buffer = malloc(sizeof(char) * (MEMORY_SIZE * 4));

	for (int i = 0; i < MEMORY_SIZE; i++) {
		sprintf(buffer+(i*4), "%03d\n", memory[i]);
	}

	return buffer;
}

#endif
