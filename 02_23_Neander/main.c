#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG 1
#define MEMORY_SIZE 256

enum Tokens {
	NOP = 0,
	STA = 16,
	LDA = 32,
	ADD = 48,
	OR 	= 64,
	AND = 80,
	NOT = 96,
	JMP = 128,
	JN	= 144,
	JZ	= 160,
	HLT = 240
};

typedef struct Program {
	int accumulator;
	int programCounter;
	int memory[MEMORY_SIZE];
	int flagN;
	int flagZ;
	int access;
	int instructions;
} TProgram;

void debugMessage(char* message) {
	if (DEBUG)
		printf("%s\n", message);
}

void debugMemory(TProgram *program) {
	for (int i = 0; i < MEMORY_SIZE; i++)
		printf("Memory[%03d]: %03d\n", i, program->memory[i]);
}

void debugProgram(TProgram *program) {
	printf("Accumulator: %03d\n", program->accumulator);
	printf("Program Counter: %03d\n", program->programCounter);

	printf("Flag N: %d\n", program->flagN);
	printf("Flag Z: %d\n", program->flagZ);

	printf("Access Count: %d\n", program->access);
	printf("Instructions: %d\n\n", program->instructions);
}

void setProgramMemoryWithFile(TProgram *program, char* programFile) {
	FILE *filePtr = fopen(programFile, "r");

	if (filePtr == NULL) {
		printf("Unable to open file!");
		exit(1);
	}

	int memoryIndex = 0;
	int currentFileChar = 0;

	char line[4];
	int  lineIndex = 0;

	while ((currentFileChar = fgetc(filePtr)) != EOF) {		
		if (currentFileChar == '\n') {
			int data = 0;
			sscanf(line, "%d", &data);

			if (data > 255) {
				printf("Invalid data at line %d\nError: data must be less than 256\n", memoryIndex + 1);
				exit(1);
			}
			
			program->memory[memoryIndex] = data;

			memset(line, '\0', sizeof(line));
			lineIndex = 0;
			memoryIndex++;
		} else {
			line[lineIndex] = currentFileChar;
			lineIndex++;
		}
	}

	while (memoryIndex < MEMORY_SIZE) {
		program->memory[memoryIndex] = 0;
		memoryIndex++;
	}

	fclose(filePtr);
}

void updateFlagN(TProgram* program) {
	if (program->accumulator >= 127)
		program->flagN = 1;
	else
		program->flagN = 0;
}

void updateFlagZ(TProgram* program) {
	if (program->accumulator == 0)
		program->flagZ = 1;
	else
		program->flagZ = 0;
}

void updateFlags(TProgram* program) {
	updateFlagN(program);
	updateFlagZ(program);
}

int getMemoryValue(TProgram* program, int address) {
	program->access++;
	return program->memory[address];
}

int getCurrentMemoryValue(TProgram* program) {
	return getMemoryValue(program, program->programCounter);
}

int getNextMemoryValue(TProgram* program) {
	return getMemoryValue(program, program->programCounter + 1);
}

void setMemoryValue(TProgram* program, int address, int value) {
	program->access++;

	if (value < 0)
		value += 256;

	program->memory[address] = value;
}

void skipNextMemorySlot(TProgram* program) {
	program->programCounter += 1;
}

int nextAsAddressToValue(TProgram* program) {
	int value = getMemoryValue(program, getNextMemoryValue(program));

	if (value < 0)
		value += 256;
	
	skipNextMemorySlot(program);

	return value;
}

void incrementProgramCounter(TProgram* program) {
	program->programCounter += 1;
}

void incrementInstructionsCount(TProgram* program) {
	program->instructions += 1;
}

void jumpAndDecrementProgramCounter(TProgram* program) {
	program->programCounter = getNextMemoryValue(program);
	program->programCounter--;
}

void normalizeAccumulator(TProgram* program) {
	if (program->accumulator >= 256)
		program->accumulator -= 256;
	else if (program->accumulator < 0)
		program->accumulator += 256;
}

TProgram createProgram(char* programFile) {
	TProgram program;

	program.access = 0;
	program.accumulator = 0;
	program.programCounter = 0;
	program.instructions = 0;
	
	setProgramMemoryWithFile(&program, programFile);
	updateFlags(&program);

	return program;
}

void executeProgram(TProgram* program) {
	int currentOperation = getCurrentMemoryValue(program);

	while (currentOperation != HLT) {
		if (currentOperation == STA) {
			debugMessage("STA operation");
			incrementInstructionsCount(program);

			int address = getNextMemoryValue(program);
			setMemoryValue(program, address, program->accumulator);
			skipNextMemorySlot(program);
		}

		if (currentOperation == LDA) {
			debugMessage("LDA operation");
			incrementInstructionsCount(program);
			program->accumulator = nextAsAddressToValue(program);
		}

		if (currentOperation == ADD) {
			debugMessage("ADD operation");
			incrementInstructionsCount(program);
			program->accumulator += nextAsAddressToValue(program);
		}

		if (currentOperation == OR) {
			debugMessage("OR operation");
			incrementInstructionsCount(program);
			program->accumulator |= nextAsAddressToValue(program);
		}

		if (currentOperation == AND) {
			debugMessage("AND operation");
			incrementInstructionsCount(program);
			program->accumulator &= nextAsAddressToValue(program);
		}

		if (currentOperation == NOT) {
			debugMessage("NOT operation");
			incrementInstructionsCount(program);
			program->accumulator = ~program->accumulator;
		}

		if (currentOperation == JMP) {
			debugMessage("JMP operation");
			incrementInstructionsCount(program);
			jumpAndDecrementProgramCounter(program);
		}

		if (currentOperation == JN) {
			debugMessage("JN operation");
			incrementInstructionsCount(program);

			if (program->flagN == 1)
				jumpAndDecrementProgramCounter(program);
			else
				skipNextMemorySlot(program);
		}

		if (currentOperation == JZ) {
			debugMessage("JZ operation");
			incrementInstructionsCount(program);

			if (program->flagZ == 1)
				jumpAndDecrementProgramCounter(program);
			else
				skipNextMemorySlot(program);
		}

		if (program->programCounter >= MEMORY_SIZE) {
			printf("Program Counter out of bounds!");
			exit(1);
		}

		normalizeAccumulator(program);
		updateFlags(program);
		incrementProgramCounter(program);
		currentOperation = getCurrentMemoryValue(program);
	}

	incrementInstructionsCount(program);
	incrementProgramCounter(program);

	printf("Program executed successfully!\n\n");
	debugMemory(program);
	printf("\n");
	debugProgram(program);
	printf("\n");
}

int main(int argc, char *argv[]) {
	TProgram program = createProgram(argv[1]);

	debugMemory(&program);
	executeProgram(&program);

  return 0;
}
