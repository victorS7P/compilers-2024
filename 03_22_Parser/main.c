#include "../03_15_Lexer/main.c"

enum OperationSymbol {
  _,
  SUM,
  SUB
};

char* getOperationSymbolNumber(enum OperationSymbol operationSymbol) {
  if (operationSymbol == SUM)
    return "ADD";
  if (operationSymbol == SUB)
    return "ADD";
}

void createAssemblyFromTokens(TToken* tokens, char* outputFilePath) {
  int literalDataLength = 0;
  int* literalData = malloc(sizeof(int) * literalDataLength);

  int operationsLength = 0;
  enum OperationSymbol* operationsData = malloc(sizeof(enum OperationSymbol) * literalDataLength);

  int i = 0;
  int invertNext = 0;
  while (tokens[i].symbol != FILE_END) {
    if (tokens[i].symbol == LITERAL_VALUE) {
      literalData = realloc(literalData, sizeof(int) * ++literalDataLength);

      int value = atoi(tokens[i].value);
      if (invertNext) {
        value = (256 - value);
        invertNext = 0;
      }

      literalData[literalDataLength - 1] = value;
    }

    else if (tokens[i].symbol == SUM_OP) {
      operationsData = realloc(operationsData, sizeof(enum OperationSymbol) * ++operationsLength);
      operationsData[operationsLength - 1] = SUM;
    }

    else if (tokens[i].symbol == SUB_OP) {
      invertNext = 1;
      operationsData = realloc(operationsData, sizeof(enum OperationSymbol) * ++operationsLength);
      operationsData[operationsLength - 1] = SUB;
    }

    i++;
  }

  FILE *fp = fopen(outputFilePath, "wb");
	if (fp != NULL)
	{
    fputs("USE\n", fp);

    int firstMemorySlot = 128;
    for (int j = 0; j < literalDataLength; j++) {
      fputs("\t", fp);
      fprintf(fp, "%03d", firstMemorySlot++);
      fputs(" ", fp);
      fprintf(fp, "%03d", literalData[j]);
      fputs("\n", fp);
    }

    fputs("\nSTART\n", fp);

    firstMemorySlot = 128;
    fputs("\tLDA ", fp);
    fprintf(fp, "%03d", firstMemorySlot++);
    fputs("\n", fp);

    for (int j = 0; j < operationsLength; j++) {
      fputs("\t", fp);
      fprintf(fp, "%s", getOperationSymbolNumber(operationsData[j]));
      fputs(" ", fp);
      fprintf(fp, "%03d", firstMemorySlot++);
      fputs("\n", fp);
    }

    fputs("\tHLT\n", fp);
		fclose(fp);
	}
}

int parser_main (int argc, char* argv[]) {
  TToken* tokens = readTokensFromFile(argv[1]);

  createAssemblyFromTokens(tokens, "./out/program.txt");
	debugTokens(tokens);

  return 0;
}
