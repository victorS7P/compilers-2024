#ifndef UTILS_H
#define UTILS_H

void debug(char* message, ...);
void debugMemory(int* memory);

void readFileContent(char* programFile, char* buffer);
void writeContentToFile(char* filePath, char* buffer);

int isBlankSymbol(char c);
void skipBlankSymbols(char* buffer, int* currentCharIndex);

char validateNumberChar(char c);
int convertValueStrToNumber(char* valueStr);

#endif
