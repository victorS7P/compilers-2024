#include <stdio.h>
#include <ctype.h>

#define DEBUG 1
#define MEMORY_SIZE 256

void debugMemory(int* memory) {
	for(int i = 0; i < MEMORY_SIZE; i++)
		printf("[%03d] = %03d\n", i, memory[i]);
	printf("\n");
}

int isBlankSymbol(char c) {
	if (
		c == '\r' ||
		c == '\n' ||
		c == '\t' ||
		c == '\0' ||
		c == ' '
	) { return 1; }

	return 0;
}

void skipBlankSymbols(FILE* filePtr) {
	char c = 0;
	while (isBlankSymbol(c = fgetc(filePtr)) == 1)
		continue;
	ungetc(c, filePtr);
}


int getNextBlockDefinition(FILE* filePtr) {
	skipBlankSymbols(filePtr);
	
	char startChar = fgetc(filePtr);
	char* tmp = (char*) malloc(sizeof(char) * 5);

	if (startChar == 'U') {
		if (
			(tmp[0] = fgetc(filePtr)) == 'S' &&
			(tmp[1] = fgetc(filePtr)) == 'E'
		) {
			free(tmp);
			return 1;
		}
		else {
			ungetc(tmp[1], filePtr);
			ungetc(tmp[0], filePtr);
		}
	}

	else if (startChar == 'D') {
		if (
			(tmp[0] = fgetc(filePtr)) == 'E' && 
			(tmp[1] = fgetc(filePtr)) == 'F' &&
			(tmp[2] = fgetc(filePtr)) == 'I' &&
			(tmp[3] = fgetc(filePtr)) == 'N' &&
			(tmp[4] = fgetc(filePtr)) == 'E'
		) {
			free(tmp);
			return 2;
		} else {
			ungetc(tmp[4], filePtr);
			ungetc(tmp[3], filePtr);
			ungetc(tmp[2], filePtr);
			ungetc(tmp[1], filePtr);
			ungetc(tmp[0], filePtr);
		}
	}

	else if (startChar == 'S') {
		if (
			(tmp[0] = fgetc(filePtr)) == 'T' && 
			(tmp[1] = fgetc(filePtr)) == 'A' &&
			(tmp[2] = fgetc(filePtr)) == 'R' &&
			(tmp[3] = fgetc(filePtr)) == 'T'
		) {
			free(tmp);
			return 3;
		} else {
			ungetc(tmp[3], filePtr);
			ungetc(tmp[2], filePtr);
			ungetc(tmp[1], filePtr);
			ungetc(tmp[0], filePtr);
		}
	}

	ungetc(startChar, filePtr);
	free(tmp);
	return -1;
}

void validateNumberChar(char c) {
	if (!isdigit(c)) {
		printf("\nERROR!\nInvalid Memory Definition.\nError: %c is not a number\n", c);
		exit(1);
	}
}

int convertValuStrToNumber(char* valueStr) {
	int value = 0;
	sscanf(valueStr, "%d", &value);
	free(valueStr);

	if (value < 0 || value > 255) {
		printf("\nERROR!\nInvalid Memory Definition!\nError: %d is not a valid value.\n", value);
		exit(1);
	}

	return value;
}

int getNext3DigitsNumber(FILE* filePtr) {
	skipBlankSymbols(filePtr);

	char* valueStr = malloc(sizeof(char) * 3);

	valueStr[0] = fgetc(filePtr);
	validateNumberChar(valueStr[0]);
	valueStr[1] = fgetc(filePtr);
	validateNumberChar(valueStr[1]);
	valueStr[2] = fgetc(filePtr);
	validateNumberChar(valueStr[2]);

	return convertValuStrToNumber(valueStr);
}

void setMemoryData(int* memory, int address, int value) {
	if (memory[address] != 0)
		printf("\nWARNING!\nMemory address %03d is already defined.\nOverwriting it with %03d\n\n", address, value);

	if (DEBUG) printf("Setting memory [%03d] = %03d\n", address, value);
	memory[address] = value;
}

void setNextMemoryData(FILE* filePtr, int* memory) {
	skipBlankSymbols(filePtr);

	int memorySlot = getNext3DigitsNumber(filePtr);

	if (fgetc(filePtr) != ' ') {
		printf("ERROR!\nInvalid Memory Definition!\nError: Must have a space between memory slot and value.\n");
		exit(1);
	}

	int value = getNext3DigitsNumber(filePtr);
	setMemoryData(memory, memorySlot, value);
}

int getNextOperaion(FILE* filePtr) {
	skipBlankSymbols(filePtr);

	int value = -1;
	char* valueStr = malloc(sizeof(char) * 2);
	valueStr[0] = fgetc(filePtr);
	valueStr[1] = fgetc(filePtr);

	if (strcmp(valueStr, "OR") == 0)
		value = 64;
	else if (strcmp(valueStr, "JN") == 0)
		value = 144;
	else if (strcmp(valueStr, "JZ") == 0)
		value = 160;

	valueStr = realloc(valueStr, sizeof(char) * 3);
	valueStr[2] = fgetc(filePtr);

	if (strcmp(valueStr, "NOP") == 0)
		value = 0;
	else if (strcmp(valueStr, "STA") == 0)
		value = 16;
	else if (strcmp(valueStr, "LDA") == 0)
		value = 32;
	else if (strcmp(valueStr, "ADD") == 0)
		value = 48;
	else if (strcmp(valueStr, "AND") == 0)
		value = 80;
	else if (strcmp(valueStr, "NOT") == 0)
		value = 96;
	else if (strcmp(valueStr, "JMP") == 0)
		value = 128;
	else if (strcmp(valueStr, "HLT") == 0)
		value = 240;

	if (value == -1) {
		printf("\nERROR!\nInvalid Definition!\nError: %s is not a valid operation.\n", valueStr);
		exit(1);
	}

	free(valueStr);
	return value;
}

int defineFunction(FILE* filePtr, int* memory, int startAddress) {
	int nextBlock = -1;

	int currentAddress = startAddress;
	while ((nextBlock = getNextBlockDefinition(filePtr)) == -1) {
		int operation = getNextOperaion(filePtr);
		setMemoryData(memory, currentAddress++, operation);
		
		if (operation != 0 && operation != 96 && operation != 240) {
			int param = getNext3DigitsNumber(filePtr);
			setMemoryData(memory, currentAddress++, param);
		}

		skipBlankSymbols(filePtr);
	}

	return nextBlock;
}

void fetchMemoryFromFile(int* memory, char* file) {
	FILE *filePtr = fopen(file, "r");

	if (filePtr == NULL) {
		printf("ERROR!\nUnable to open file!\n");
		exit(1);
	}

	char currentFileChar = 0;
	skipBlankSymbols(filePtr);

	if (getNextBlockDefinition(filePtr) == 1) {
		if (DEBUG) printf("Processing USE block\n");

		skipBlankSymbols(filePtr);
		while (isdigit(currentFileChar = fgetc(filePtr))) {
			ungetc(currentFileChar, filePtr);
			setNextMemoryData(filePtr, memory);
			skipBlankSymbols(filePtr);
		}

		if (DEBUG) printf("\n");
	}

	ungetc(currentFileChar, filePtr);
	int nextBlock = getNextBlockDefinition(filePtr);
	while (nextBlock == 2) {
		skipBlankSymbols(filePtr);
		int startAddress = getNext3DigitsNumber(filePtr);
		if (DEBUG) printf("Definig function at address %03d\n", startAddress);

		nextBlock = defineFunction(filePtr, memory, startAddress);
		if (DEBUG) printf("\n");
	}

	if (nextBlock == 3) {
		if (DEBUG) printf("Processing START block\n");

		int currentAddress = 0;
		while ((currentFileChar = fgetc(filePtr)) != EOF) {
			ungetc(currentFileChar, filePtr);

			int operation = getNextOperaion(filePtr);
			setMemoryData(memory, currentAddress++, operation);

			if (operation != 0 && operation != 96 && operation != 240) {
				int param = getNext3DigitsNumber(filePtr);
				setMemoryData(memory, currentAddress++, param);
			}

			skipBlankSymbols(filePtr);
		}
		if (DEBUG) printf("\n");
	}

	fclose(filePtr);
}

void saveMemoryDataToFile(int* memory, char* filePath) {
	FILE *fp = fopen(filePath, "wb");

	if (fp != NULL)
	{
		for(int i = 0; i < MEMORY_SIZE; i++)
			fprintf(fp, "%03d\n", memory[i]);
		fputs("\n", fp);

		fclose(fp);
	}
}

int main(int argc, char *argv[]) {
	int memory[MEMORY_SIZE];
	for (int i = 0; i < MEMORY_SIZE; i++)
		memory[i] = 0;
	
  fetchMemoryFromFile(&memory, argv[1]);
	saveMemoryDataToFile(memory, "./out/program.txt");
	
  return 0;
}