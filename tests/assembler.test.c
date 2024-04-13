#ifndef ASSEMBLER_T
#define ASSEMBLER_T

#include "./__utils.test.c"
#include "../src/modules/assembler/assembler.c"

void getNextBlockDefinitionTest() {
  printf("#getNextBlockDefinition\n");

  int currentCharIndex = 0;
  int result;

  result = getNextBlockDefinition("USE", &currentCharIndex);
  assert(result == USE, "Should recognize USE keyworkd");
  assert(currentCharIndex == 3, "Should update currentCharIndex when recognize USE keyworkd");

  currentCharIndex = 0;
  result = getNextBlockDefinition("   START", &currentCharIndex);
  assert(result == START, "Should recognize START keyworkd");
  assert(currentCharIndex == 8, "Should update currentCharIndex when recognize START keyworkd");

  currentCharIndex = 0;
  result = getNextBlockDefinition("\n\tDEFINE\n\t", &currentCharIndex);
  assert(result == DEFINE, "Should recognize DEFINE keyworkd");
  assert(currentCharIndex == 8, "Should update currentCharIndex when recognize DEFINE keyworkd");

  currentCharIndex = 0;
  result = getNextBlockDefinition("USI", &currentCharIndex);
  assert(result == -1, "Should recognize an invalid keyworkd");
  assert(currentCharIndex == 0, "Should not update currentCharIndex when recognize an invalid keyworkd");

  currentCharIndex = 0;
  result = getNextBlockDefinition("LDA 020", &currentCharIndex);
  assert(result == -1, "Should recognize an invalid keyworkd");
  assert(currentCharIndex == 0, "Should not update currentCharIndex when recognize an invalid keyworkd");

  printf("\n");
}

void getNext3DigitsNumberTest() {
  printf("#getNext3DigitsNumber\n");

  int currentCharIndex = 0;
  int result;

  result = getNext3DigitsNumber("   128 256", &currentCharIndex);
  assert(result == 128, "Should get next number as 3 digits decimal");
  assert(currentCharIndex == 6, "Should update currentCharIndex when get the number");

  printf("\n");
}

void setNextMemoryDataTest() {
  printf("#setNextMemoryData\n");

  int* memory = malloc(sizeof(int) * 256);
  int currentCharIndex = 0;

  setNextMemoryData(memory, "100 220", &currentCharIndex);
  assert(currentCharIndex == 7, "Should update currentCharIndex when set the memory");

  currentCharIndex = 0;
  setNextMemoryData(memory, "\n\n\t  120 100", &currentCharIndex);
  assert(currentCharIndex == 12, "Should update currentCharIndex when set the memory");

  assert(memory[100] == 220, "Should properly set memory value");
  assert(memory[120] == 100, "Should properly set memory value");

  free(memory);
  printf("\n");
}

void getNextOperaionTest() {
  printf("#getNextOperaion\n");

  int currentCharIndex = 0;
  int result;

  result = getNextOperaion("OR", &currentCharIndex);
  assert(result == OR, "Should recognize OR keyworkd");
  assert(currentCharIndex == 2, "Should update currentCharIndex when recognize USE keyworkd");

  result = getNextOperaion("  \tJMP", &currentCharIndex);
  assert(result == JMP, "Should recognize JMP keyworkd");
  assert(currentCharIndex == 6, "Should update currentCharIndex when recognize USE keyworkd");

  printf("\n");
}

void defineFunctionTest() {
  printf("#defineFunction\n");

  int* memory = malloc(sizeof(int) * 256);
  int currentCharIndex = 0;
  int result = 0;

  result = defineFunction(memory, "\n\tLDA 128\n\tADD 129\n\tSTA 130\n\tNOT\nSTART", &currentCharIndex, 10);
  assert(result == START, "Should return next instruction block");
  assert(memory[10] == LDA, "Should set instruction in memory");
  assert(memory[11] == 128, "Should set value in memory");
  assert(memory[16] == NOT, "Should set instruction in memory");
  assert(currentCharIndex == 38, "Should update currentCharIndex when define function");

  currentCharIndex = 0;
  result = defineFunction(memory, "\n\tLDA 128\nDEFINE", &currentCharIndex, 0);
  assert(result == DEFINE, "Should return next instruction block");
  assert(memory[1] == 128, "Should set instruction in memory");
  assert(currentCharIndex == 16, "Should update currentCharIndex when definie function");

  free(memory);
  printf("\n");
}

void fetchMemoryFromBufferTest() {
  printf("#fetchMemoryFromBuffer\n");

  int memory[MEMORY_SIZE];
	for (int i = 0; i < MEMORY_SIZE; i++)
		memory[i] = 0;
  char* memoryString = malloc(0);

  readFileContent("./tests/resources/assembler_input.txt", memoryString);
  fetchMemoryFromBuffer(memory, memoryString);

  assert(memory[128] ==   5, "Should create data in memory");
  assert(memory[129] ==   7, "Should create data in memory");
  assert(memory[ 64] == LDA, "Should create function in memory");
  assert(memory[ 65] == 128, "Should create function in memory");
  assert(memory[ 70] == HLT, "Should create function in memory");
  assert(memory[  0] == JMP, "Should create start block in the init of the memory");
  assert(memory[  1] ==  64, "Should create start block in the init of the memory");
  assert(memory[  2] == HLT, "Should create start block in the init of the memory");

  free(memoryString);
  printf("\n");
}

void saveMemoryToBufferTest() {
  printf("#saveMemoryToBuffer\n");

  int memory[MEMORY_SIZE];
	for (int i = 0; i < MEMORY_SIZE; i++)
		memory[i] = 0;

  memory[0] = 120;
  memory[99] = 30;
  memory[255] = 100;

  char* result = saveMemoryToBuffer(memory);
  assert(result[0] == '1', "Should create buffer from memory");
  assert(result[1] == '2', "Should create buffer from memory");
  assert(result[2] == '0', "Should create buffer from memory");

  assert(result[396] == '0', "Should create buffer from memory");
  assert(result[397] == '3', "Should create buffer from memory");
  assert(result[398] == '0', "Should create buffer from memory");

  assert(result[1020] == '1', "Should create buffer from memory");
  assert(result[1021] == '0', "Should create buffer from memory");
  assert(result[1022] == '0', "Should create buffer from memory");
  assert(result[1023] == '\n', "Should create buffer from memory");
  assert(strlen(result) == 1024, "Should create buffer from memory");

  free(result);
  printf("\n");
}

int main(int argc, char const *argv[]) {
  getNextBlockDefinitionTest();
  getNext3DigitsNumberTest();
  getNextOperaionTest();

  setNextMemoryDataTest();
  defineFunctionTest();
  fetchMemoryFromBufferTest();
  saveMemoryToBufferTest();

  return 0;
}

#endif;