#ifndef PARSER_H
#define PARSER_H

#include "../lexer/lexer.h"

typedef enum ParserGrammarSymbol
{
	_,
  BNF_EXPRESSION,
	BNF_FILE_END,
  BNF_OPERATOR,
  BNF_LITERAL_VALUE
} TParserGrammarSymbol;

typedef struct ParserTree
{
  TParserGrammarSymbol symbol;
  TLexerToken token;
  struct ParserTree* childrenTree;
  int childrenLength;
  int isTerminal;
} TParserTree;

typedef struct ParserAssemblerData
{
  int* literalValues;
  int  literalValuesLength;
  int* operations;
  int  operationsLength;
} TParserAssemblerData;

const char * const ParserGrammarSymbolStr[] =
{
	[BNF_EXPRESSION]	      = "BNF_EXPRESSION",
	[BNF_FILE_END]	      	= "BNF_FILE_END",
	[BNF_OPERATOR]		      = "BNF_OPERATOR",
	[BNF_LITERAL_VALUE]			= "BNF_LITERAL_VALUE"
};

void debugTree(TParserTree* tree, int depth);

TParserTree* initTree();
int parseExpression(TParserTree* root, TLexerToken* tokens, int* currentTokenIndex, TParserAssemblerData* assemblerData);
int parseLiteralValue(TParserTree* root, TLexerToken* tokens, int* currentTokenIndex, TParserAssemblerData* assemblerData);
int parseFileEnd(TParserTree* root, TLexerToken* tokens, int* currentTokenIndex, TParserAssemblerData* assemblerData);
int parseOperator(TParserTree* root, TLexerToken* tokens, int* currentTokenIndex, TParserAssemblerData* assemblerData);

TParserAssemblerData* initAssemblerData();
char* createAssemblyFromData(TParserAssemblerData* assemblerData);

#endif
