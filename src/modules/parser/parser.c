#ifndef PARSER
#define PARSER

#include "./parser.h"

#include "../lexer/lexer.c"
#include "../../utils/utils.c"

void debugTree(TParserTree* tree, int depth) {
  if (depth > 0)
  {
    if (depth == 1)
      printf("|--");

    else
    {
      printf("|");

      for (int i = 1; i < depth; i++)
				printf("  ");

      printf("|--");
    }
  }

  if (tree->symbol)
  {
    printf("<%s>", ParserGrammarSymbolStr[tree->symbol]);

    if (tree->isTerminal)
      printf(" (%s)\n", tree->token.value);
    else
      printf("\n");
  }

  if (tree->childrenLength)
  {
    depth++;

    for (int i = 0; i < tree->childrenLength; i++)
      debugTree(&(tree->childrenTree[i]), depth);
  }
}

int parseFileEnd(TParserTree* root, TLexerToken* tokens, int* currentTokenIndex, TParserAssemblerData* assemblerData) {
	root->token = tokens[(*currentTokenIndex)++];

	if (root->token.symbol == FILE_END) {
		root->symbol = BNF_FILE_END;
    root->isTerminal = 1;

		assemblerData->operations = realloc(assemblerData->operations, sizeof(int) * ++assemblerData->operationsLength);
		assemblerData->operations[assemblerData->operationsLength - 1] = root->token.symbol;

		return 1;
	}

	(*currentTokenIndex)--;
	return 0;
}

int parseLiteralValue(TParserTree* root, TLexerToken* tokens, int* currentTokenIndex, TParserAssemblerData* assemblerData) {
	root->token = tokens[(*currentTokenIndex)++];

	if (root->token.symbol == LITERAL_VALUE) {
		root->symbol = BNF_LITERAL_VALUE;
    root->isTerminal = 1;

		assemblerData->literalValues = realloc(assemblerData->literalValues, sizeof(int) * ++assemblerData->literalValuesLength);
		assemblerData->literalValues[assemblerData->literalValuesLength - 1] = atoi(root->token.value);

		return 1;
	}

	(*currentTokenIndex)--;
	return 0;
}

int parseOperator(TParserTree* root, TLexerToken* tokens, int* currentTokenIndex, TParserAssemblerData* assemblerData) {
	root->token = tokens[(*currentTokenIndex)++];

	if (
		root->token.symbol == SUM_OP ||
		root->token.symbol == SUB_OP ||
		root->token.symbol == MUL_OP ||
		root->token.symbol == DIV_OP
	) {
		root->symbol = BNF_OPERATOR;
    root->isTerminal = 1;

		assemblerData->operations = realloc(assemblerData->operations, sizeof(int) * ++assemblerData->operationsLength);
		assemblerData->operations[assemblerData->operationsLength - 1] = root->token.symbol;

		return 1;
	}

	(*currentTokenIndex)--;
	return 0;
}

int parseExpression(TParserTree* root, TLexerToken* tokens, int* currentTokenIndex, TParserAssemblerData* assemblerData) {
	int startTokenIndex = (*currentTokenIndex);

	root->symbol = BNF_EXPRESSION;
	root->isTerminal = 0;

	TParserTree* childrenTree = malloc(sizeof(TParserTree));

	if (parseLiteralValue(childrenTree, tokens, currentTokenIndex, assemblerData)) {
		childrenTree = realloc(childrenTree, sizeof(TParserTree) * 2);

		if (parseFileEnd(&(childrenTree[1]), tokens, currentTokenIndex, assemblerData)) {
			root->childrenTree = childrenTree;
			root->childrenLength = 2;
			return 1;
		};

		if (parseOperator(&(childrenTree[1]), tokens, currentTokenIndex, assemblerData)) {
			childrenTree = realloc(childrenTree, sizeof(TParserTree) * 3);

			if (parseExpression(&(childrenTree[2]), tokens, currentTokenIndex, assemblerData)) {
				root->childrenTree = childrenTree;
				root->childrenLength = 3;
				return 1;
			}
		}
	}

	(*currentTokenIndex) = startTokenIndex;
	return 0;
}

TParserTree* initTree() {
	TParserTree* tree = malloc(sizeof(TParserTree));
	tree[0].childrenLength = 0;
	tree[0].isTerminal = 0;

	return tree;
}

TParserAssemblerData* initAssemblerData() {
	TParserAssemblerData* assemblerData = malloc(sizeof(TParserAssemblerData));
	assemblerData->literalValues = malloc(0);
	assemblerData->literalValuesLength = 0;
	assemblerData->operations = malloc(0);
	assemblerData->operationsLength = 0;
	return assemblerData;
}

char* createAssemblyFromData(TParserAssemblerData* assemblerData) {
	int valueMemorySlot = (MEMORY_SIZE/2);
	char* buffer = malloc(sizeof(char) * 1024);

	int bufferLen = sprintf(buffer, "USE\n");

	int subOperationValueMemory = 254;
	bufferLen += sprintf(buffer + bufferLen, "\t%03d 001\n", subOperationValueMemory);

	for (int i = 0; i < assemblerData->literalValuesLength; i++) {
		bufferLen += sprintf(buffer + bufferLen, "\t%03d ", valueMemorySlot++);
		bufferLen += sprintf(buffer + bufferLen, "%03d\n", assemblerData->literalValues[i]);
	}

	int tempMemorySlot = valueMemorySlot;
	valueMemorySlot = (MEMORY_SIZE/2);
	bufferLen += sprintf(buffer + bufferLen, "START\n");
	bufferLen += sprintf(buffer + bufferLen, "\tLDA %03d\n", valueMemorySlot++);

	for (int j = 0; j < assemblerData->operationsLength; j++) {
		if(assemblerData->operations[j] == SUM_OP) {
			bufferLen += sprintf(buffer + bufferLen, "\tADD %03d\n", valueMemorySlot++);
		}

		if(assemblerData->operations[j] == SUB_OP) {
			bufferLen += sprintf(buffer + bufferLen, "\tSTA %03d\n", tempMemorySlot);
			bufferLen += sprintf(buffer + bufferLen, "\tLDA %03d\n", valueMemorySlot++);
			bufferLen += sprintf(buffer + bufferLen, "\tNOT\n");
			bufferLen += sprintf(buffer + bufferLen, "\tADD %03d\n", subOperationValueMemory);
			bufferLen += sprintf(buffer + bufferLen, "\tADD %03d\n", tempMemorySlot);
		}

		if(assemblerData->operations[j] == FILE_END) {
			bufferLen += sprintf(buffer + bufferLen, "\tHLT\n");
		}
	}

	buffer = realloc(buffer, sizeof(char) * bufferLen);
	return buffer;
}

#endif
