#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
