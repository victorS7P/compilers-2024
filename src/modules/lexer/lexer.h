#ifndef LEXER_H
#define LEXER_H

typedef enum LexerTokenSymbol {
	UNKNOWN,
	BLANK,
	NEW_LINE,
	PARENTHESIS_OPEN,
	PARENTHESIS_CLOSE,
	SUM_OP,
	SUB_OP,
	MUL_OP,
	DIV_OP,
	LITERAL_VALUE,
	FILE_END
} TLexerTokenSymbol;

const char * const TokenSymbolStr[] =
{
	[UNKNOWN] 					= "UNKNOWN",
	[BLANK]							= "BLANK",
	[NEW_LINE]					= "NEW_LINE",
	[PARENTHESIS_OPEN] 	= "PARENTHESIS_OPEN",
	[PARENTHESIS_CLOSE] = "PARENTHESIS_CLOSE",
	[SUM_OP]  					= "SUM_OP",
	[SUB_OP]  					= "SUB_OP",
	[MUL_OP]  					= "MUL_OP",
	[DIV_OP]  					= "DIV_OP",
	[LITERAL_VALUE] 		= "LITERAL_VALUE",
	[FILE_END]					= "FILE_END"
};

typedef struct LexerToken {
	int line;
	TLexerTokenSymbol symbol;
	char* value;
  int valueLen;
	int columnStart;
} TLexerToken;

void debugToken(TLexerToken token);
void debugTokens(TLexerToken* tokens);

int TokenAllowMultipleValues(TLexerToken token);
TLexerToken createTokenFromChar(char c);

TLexerToken getTokensWithSameSymbol(char* buffer, int* currentCharIndex);
TLexerToken* fetchTokensFromBuffer (char* buffer);

int addTokenToBuffer(TLexerToken token, char** buffer, int bufferLen);
char* createBufferWithTokensData(TLexerToken* tokens);

#endif
