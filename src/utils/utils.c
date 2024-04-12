#ifndef UTILS
#define UTILS

#include "../config/config.c"

void debug(char* message, ...) {
	va_list args;
	va_start(args, message);

	if (DEBUG) vprintf(message, args);
	va_end(args);
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

#endif
