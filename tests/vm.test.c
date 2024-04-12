#ifndef VM_T
#define VM_T

#include "./__utils.test.c"
#include "../src/vm/vm.c"

void createNeanderVMTest() {
  printf("#createNeanderVMTest\n");
  TNeanderVM vm = createNeanderVM();

  assert(vm.accumulator == 0, "Should create a VM with accumulator = 0");
	assert(vm.programCounter == 0, "Should create a VM with programCounter = 0");
	assert(vm.accessesCounter == 0, "Should create a VM with accessesCounter = 0");
	assert(vm.instructionsCounter == 0, "Should create a VM with instructionsCounter = 0");
  assert(vm.flagZ == 1, "Should create a VM with an initial flagZ");
  assert(vm.flagN == 0, "Should create a VM with an initial flagN");

  printf("\n");
}

void updateFlagNTest() {
  printf("#updateFlagNTest\n");
  TNeanderVM vm = createNeanderVM();

  vm.accumulator = randomNumber(0, 127);
  updateFlagN(&vm);
  assert(vm.flagN == 0, "FlagN should be 0 when accumulator == %d", vm.accumulator);

  vm.accumulator = 127;
  updateFlagN(&vm);
  assert(vm.flagN == 1, "FlagN should be 1 when accumulator == 127");

  vm.accumulator = randomNumber(128, 255);
  updateFlagN(&vm);
  assert(vm.flagN == 1, "FlagN should be 1 when accumulator == %d", vm.accumulator);

  printf("\n");
}

void updateFlagZTest() {
  printf("#updateFlagZTest\n");
  TNeanderVM vm = createNeanderVM();

  vm.accumulator = randomNumber(1, 255);
  updateFlagZ(&vm);
  assert(vm.flagZ == 0, "FlagZ should be 0 when accumulator == %d", vm.accumulator);

  vm.accumulator = 0;
  updateFlagZ(&vm);
  assert(vm.flagZ == 1, "FlagZ should be 1 when accumulator == 0");

  printf("\n");
}

void updateFlagsTest() {
  printf("#updateFlagsTest\n");
  TNeanderVM vm = createNeanderVM();

  vm.accumulator = 0;
  updateFlags(&vm);
  assert(vm.flagZ == 1, "Should update flagZ when accumulator == 0");
  assert(vm.flagN == 0, "Should update flagN when accumulator == 0");

  vm.accumulator = randomNumber(1, 127);
  updateFlags(&vm);
  assert(vm.flagZ == 0, "Should update flagZ when accumulator == %d", vm.accumulator);
  assert(vm.flagN == 0, "Should update flagN when accumulator == %d", vm.accumulator);

  vm.accumulator = randomNumber(128, 255);
  updateFlags(&vm);
  assert(vm.flagZ == 0, "Should update flagZ when accumulator == %d", vm.accumulator);
  assert(vm.flagN == 1, "Should update flagN when accumulator == %d", vm.accumulator);

  printf("\n");
}

void normalizeValueTest() {
  printf("#normalizeValue\n");

  int value = randomNumber(0, 255);
  assert(normalizeValue(value) == value, "Should return the same value when it's valid");
  assert(normalizeValue(-33) == 223, "Should normalize a negative numver");
  assert(normalizeValue(267) == 11, "Should normalize an overflow number");

  printf("\n");
}

void normalizeAccumulatorTest() {
  printf("#normalizeAccumulator\n");
  TNeanderVM vm = createNeanderVM();

  int value = randomNumber(0, 255);
  vm.accumulator = value;
  normalizeAccumulator(&vm);
  assert(vm.accumulator == value, "Should keep the same value when it's valid");

  vm.accumulator = -21;
  normalizeAccumulator(&vm);
  assert(vm.accumulator == 235, "Should normalize when it's a negative number");

  vm.accumulator = 257;
  normalizeAccumulator(&vm);
  assert(vm.accumulator == 1, "Should normalize when it's an overflow number");

  printf("\n");
}

void setMemoryValueTest() {
  printf("#setMemoryValue\n");
  TNeanderVM vm = createNeanderVM();

  int address = randomNumber(0, 255);
  setMemoryValue(&vm, address, address);
  assert(vm.memory[address] == address, "Should set memory value");
  assert(vm.accessesCounter == 1, "Should update memory access counter");

  int negativeValue = -20;
  setMemoryValue(&vm, address, negativeValue);
  assert(vm.memory[address] == 236, "Should normalize negative values before set in memory");
  assert(vm.accessesCounter == 2, "Should update memory access counter");

  int overflowNumber = 280;
  setMemoryValue(&vm, address, overflowNumber);
  assert(vm.memory[address] == 24, "Should normalize overflow values before set in memory");
  assert(vm.accessesCounter == 3, "Should update memory access counter");

  printf("\n");
}

void getMemoryValueTest() {
  printf("#getMemoryValue\n");
  TNeanderVM vm = createNeanderVM();

  int address = randomNumber(0, 255);
  int value = randomNumber(0, 255);
  setMemoryValue(&vm, address, value);

  assert(getMemoryValue(&vm, address) == vm.memory[address], "Should get value in given memory address");
  assert(vm.accessesCounter == 2, "Should increment memory access counter");

  printf("\n");
}

void getCurrentMemoryValueTest() {
  printf("#getCurrentMemoryValue\n");
  TNeanderVM vm = createNeanderVM();

  int address = randomNumber(0, 255);
  int value = randomNumber(0, 255);
  setMemoryValue(&vm, address, value);
  vm.programCounter = address;

  assert(getCurrentMemoryValue(&vm) == vm.memory[address], "Should get memory value for current programCounter");
  assert(vm.accessesCounter == 2, "Should increment memory access counter");

  printf("\n");
}

void getNextMemoryValueTest() {
  printf("#getNextMemoryValue\n");
  TNeanderVM vm = createNeanderVM();

  int address = randomNumber(0, 255);
  int value = randomNumber(0, 255);
  setMemoryValue(&vm, address, value);
  vm.programCounter = (address - 1);

  assert(getNextMemoryValue(&vm) == vm.memory[address], "Should get next memory value for current programCounter");
  assert(vm.accessesCounter == 2, "Should increment memory access counter");

  printf("\n");
}

void nextAsAddressToValueTest() {
  printf("#nextAsAddressToValue\n");
  TNeanderVM vm = createNeanderVM();

  int value = randomNumber(0, 255);
  int valueAddress = randomNumber(0, 255);
  int currentAddress = randomNumber(0, 255);
  vm.programCounter = currentAddress;
  vm.memory[currentAddress + 1] = valueAddress;
  vm.memory[valueAddress] = value;

  assert(nextAsAddressToValue(&vm) == value, "Should use next memory slot as an address to another memory value");
  assert(vm.accessesCounter == 2, "Should increment memory access counter");

  printf("\n");
}

void skipNextMemorySlotTest() {
  printf("#skipNextMemorySlot\n");
  TNeanderVM vm = createNeanderVM();

  int value = randomNumber(0, 255);
  vm.programCounter = value;
  skipNextMemorySlot(&vm);
  assert(vm.programCounter == (value + 1), "Should increment program counter");

  printf("\n");
}

void incrementProgramCounterTest() {
  printf("#incrementProgramCounter\n");
  TNeanderVM vm = createNeanderVM();

  int value = randomNumber(0, 255);
  vm.programCounter = value;
  incrementProgramCounter(&vm);
  assert(vm.programCounter == (value + 1), "Should increment program counter");

  printf("\n");
}

void incrementInstructionsCounterTest() {
  printf("#incrementInstructionsCounter\n");
  TNeanderVM vm = createNeanderVM();

  int value = randomNumber(0, 255);
  vm.instructionsCounter = value;
  incrementInstructionsCounter(&vm);
  assert(vm.instructionsCounter == (value + 1), "Should increment instructions counter");

  printf("\n");
}

void jumpAndDecrementProgramCounterTest() {
  printf("#jumpAndDecrementProgramCounter\n");
  TNeanderVM vm = createNeanderVM();

  int newProgramCounter = randomNumber(0, 255);
  vm.programCounter = 0;
  vm.memory[1] = newProgramCounter;
  jumpAndDecrementProgramCounter(&vm);

  assert(
    vm.programCounter == (newProgramCounter - 1),
    "Should set program counter as decremented next value in memory"
  );
  assert(vm.accessesCounter == 1, "Should update memory access counter");

  printf("\n");
}

void setNeanderVMFromStringBufferTest() {
  printf("#setNeanderVMFromStringBuffer\n");

  TNeanderVM vm = createNeanderVM();
  char* memoryString = malloc(0);

  readFileContent("./tests/resources/vm_memory_input.txt", memoryString);
  int result = setNeanderVMFromStringBuffer(&vm, memoryString);
  free(memoryString);

  assert(result == 0, "Should return a valid result");
  assert(vm.memory[0]   ==  32, "Should properly set all memory values");
  assert(vm.memory[10]  ==  16, "Should properly set all memory values");
  assert(vm.memory[129] ==   4, "Should properly set all memory values");
  assert(vm.memory[150] ==   0, "Should properly set all memory values");
  assert(vm.memory[255] ==   0, "Should properly set all memory values");

  TNeanderVM invalidVm = createNeanderVM();
  char* invalidMemoryString = malloc(0);

  readFileContent("./tests/resources/vm_memory_input_with_invalid_data.txt", invalidMemoryString);
  int invalidResult = setNeanderVMFromStringBuffer(&invalidVm, invalidMemoryString);
  free(invalidMemoryString);
  assert(invalidResult == -1, "Should return an invalid result");

  printf("\n");
}

void executeVMTest() {
  printf("#executeVM\n");

  TNeanderVM vm = createNeanderVM();
  char* memoryString = malloc(0);

  readFileContent("./tests/resources/vm_memory_input.txt", memoryString);
  setNeanderVMFromStringBuffer(&vm, memoryString);
  free(memoryString);

  int result = executeVM(&vm);
  assert(result == 0, "Should properly execute vm");
  assert(vm.memory[130] == 7, "Should properly add values in memory and store the result");
  assert(vm.memory[131] == 248, "Should properly jump, negate accumulator number and store it");
  assert(vm.accumulator == 7, "Should properly negate-jump and negate accumulator number");

  printf("\n");
}

int main(int argc, char const *argv[]) {
  createNeanderVMTest();

  updateFlagNTest();
  updateFlagZTest();
  updateFlagsTest();

  normalizeValueTest();
  normalizeAccumulatorTest();

  setMemoryValueTest();
  getMemoryValueTest();
  getNextMemoryValueTest();
  nextAsAddressToValueTest();

  skipNextMemorySlotTest();
  incrementProgramCounterTest();
  incrementInstructionsCounterTest();
  jumpAndDecrementProgramCounterTest();

  setNeanderVMFromStringBufferTest();
  executeVMTest();

  return 0;
}

#endif