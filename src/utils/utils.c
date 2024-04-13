#ifndef UTILS
#define UTILS

#include "../config/config.c"

void debug(char* message, ...) {
	va_list args;
	va_start(args, message);

	if (DEBUG) vprintf(message, args);
	va_end(args);
}

void debugMemory(int* memory) {
	for(int i = 0; i < MEMORY_SIZE; i++)
		debug("[%03d] = %03d\n", i, memory[i]);
	debug("\n");
}

void readFileContent(char* programFile, char** buffer) {
  FILE *filePtr = fopen(programFile, "rb");

	if (filePtr == NULL) {
		printf("Unable to open file: %s\n", programFile);
		exit(1);
	}

  fseek (filePtr, 0, SEEK_END);
  long length = ftell(filePtr);

  fseek (filePtr, 0, SEEK_SET);
  (*buffer) = realloc((*buffer), length);

  if (!(*buffer)) {
    printf("Unable to read file: %s\n", programFile);
		exit(1);
  }

  fread (buffer, 1, length, filePtr);
	fclose(filePtr);
}

void writeContentToFile(char* filePath, char* buffer) {
	FILE *fp = fopen(filePath, "wb");

	if (fp == NULL) {
		printf("Unable to create or write to file: %s\n", filePath);
		exit(1);
	}

	fprintf(fp, "%s", buffer);
	fclose(fp);
}

int isBlankSymbol(char c) {
	if (c == '\r' || c == '\n' || c == '\t' || c == '\0' || c == ' ')
		return 1;

	return 0;
}

void skipBlankSymbols(char* buffer, int* currentCharIndex) {
	while(isBlankSymbol(buffer[(*currentCharIndex)]))
    (*currentCharIndex)++;
}

char validateNumberChar(char c) {
	if (!isdigit(c)) {
		printf("\nERROR!\nInvalid Memory Definition.\nError: %c is not a number\n", c);
		exit(1);
	}

	return c;
}

int convertValueStrToNumber(char* valueStr) {
	int value = 0;
	sscanf(valueStr, "%d", &value);

	if (value < 0 || value > 255) {
		printf("\nERROR!\nInvalid Memory Definition!\nError: %d is not a valid value.\n", value);
		exit(1);
	}

	return value;
}

#endif
