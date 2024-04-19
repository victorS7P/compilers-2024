#ifndef ASSEMBLER_H
#define ASSEMBLER_H

typedef enum AssemblerToken {
	USE = 1,
	START = 2,
	DEFINE = 3
} TAssemblerToken;

int compareStringOrKeepCurrentCharIndex(char* buffer, int* currentCharIndex, char* string);

int getNextBlockDefinition(char* buffer, int* currentCharIndex);
int getNext3DigitsNumber(char* buffer, int* currentCharIndex);
int getNextOperaion(char* buffer, int* currentCharIndex);

void setMemoryData(int* memory, int address, int value);
void setNextMemoryData(int* memory, char* buffer, int* currentCharIndex);
int defineFunction(int* memory, char* buffer, int* currentCharIndex, int startMemoryAddress);

void fetchMemoryFromBuffer(int* memory, char* buffer);
char* saveMemoryToBuffer(int* memory);

#endif
