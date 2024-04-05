#include <stdio.h>
#include <stdlib.h>

enum TokenSymbol {
	UNKNOWN,
	BLANK,
	NEW_LINE,
	PARENTHESIS_OPEN,
	PARENTHESIS_CLOSE,
	SUM_OP,
	SUB_OP,
	MULT_OP,
	DIV_OP,
	LITERAL_VALUE,
	FILE_END
};

const char * const TokenSymbolStr[] =
{
	[UNKNOWN] 					= "UNKNOWN",
	[BLANK]							= "BLANK",
	[NEW_LINE]					= "NEW_LINE",
	[PARENTHESIS_OPEN] 	= "PARENTHESIS_OPEN",
	[PARENTHESIS_CLOSE] = "PARENTHESIS_CLOSE",
	[SUM_OP]  					= "SUM_OP",
	[SUB_OP]  					= "SUB_OP",
	[MULT_OP]  					= "MULT_OP",
	[DIV_OP]  					= "DIV_OP",
	[LITERAL_VALUE] 		= "LITERAL_VALUE",
	[FILE_END]					= "FILE_END"
};

typedef struct Token {
	enum TokenSymbol symbol;
	char* value;
	int line;
	int columnStart;
} TToken;

TToken createTokenFromChar(char c){
	TToken token;

	switch (c) {
		case '\n':
			token.symbol = NEW_LINE;
			break;

		case ' ':
		case '\t':
			token.symbol = BLANK;
			break;
		
		case '(':
			token.symbol = PARENTHESIS_OPEN;
			break;

		case ')':
			token.symbol = PARENTHESIS_CLOSE;
			break;

		case '+':
			token.symbol = SUM_OP;
			break;

		case '-':
			token.symbol = SUB_OP;
			break;

		case '*':
			token.symbol = MULT_OP;
			break;

		case '/':
			token.symbol = DIV_OP;
			break;

		case '0' ... '9':
			token.symbol = LITERAL_VALUE;
			break;

		default:
			token.symbol = UNKNOWN;
			break;
	}

	return token;
}

int appendTokensWithSameSymbol(FILE* filePtr, TToken* currentToken, enum TokenSymbol symbol) {
	int nextFileChar = fgetc(filePtr);
	if (nextFileChar == EOF) {
		ungetc(nextFileChar, filePtr);
		return 1;
	}

	TToken nextToken = createTokenFromChar(nextFileChar);
	int currentCharsInToken = 1;

	while (nextToken.symbol == symbol) {
		currentCharsInToken++;
		currentToken->value = realloc(currentToken->value, sizeof(char) * (currentCharsInToken));
		currentToken->value[currentCharsInToken - 1] = nextFileChar;

		nextFileChar = fgetc(filePtr);
		if (nextFileChar == EOF) {
			ungetc(nextFileChar, filePtr);
			break;
		}

		nextToken = createTokenFromChar(nextFileChar);
	}

	ungetc(nextFileChar, filePtr);
	return currentCharsInToken;
}

TToken* readTokensFromFile (char* file) {
	FILE *filePtr = fopen(file, "r");

	if (filePtr == NULL) {
		printf("Unable to open file %s!\n", file);
		exit(1);
	}

	int currentFileChar = 0;
	int currentFileLine = 1;
	int currentFileColumn = 1;
	
	int currentTokenIndex = 0;
	TToken* tokens = malloc(currentTokenIndex);
	while ((currentFileChar = fgetc(filePtr)) != EOF) {
		TToken token = createTokenFromChar(currentFileChar);
		token.line = currentFileLine;
		token.columnStart = currentFileColumn;

		if (token.symbol == NEW_LINE) {
			currentFileLine++;
			currentFileColumn = 1;
			continue;
		}
		
		if (token.symbol == BLANK) {
			currentFileColumn++;
			continue;
		}

		token.value = malloc(sizeof(char));
		token.value[0] = currentFileChar;

		int charsInToken = 1;
		if (token.symbol == UNKNOWN)
			charsInToken = appendTokensWithSameSymbol(filePtr, &token, UNKNOWN);
		else if (token.symbol == LITERAL_VALUE)
			charsInToken = appendTokensWithSameSymbol(filePtr, &token, LITERAL_VALUE);

		currentTokenIndex++;
		tokens = realloc(tokens, sizeof(TToken) * (currentTokenIndex));
		tokens[currentTokenIndex - 1] = token;

		currentFileColumn += charsInToken;
	}

	fclose(filePtr);

	TToken fileEndToken = {
		symbol: FILE_END,
		value: NULL,
		line: currentFileLine,
		columnStart: currentFileColumn
	};

	currentTokenIndex++;
	tokens = realloc(tokens, sizeof(TToken) * (currentTokenIndex));
	tokens[currentTokenIndex - 1] = fileEndToken;
	
	return tokens;
}

void printToken(TToken token) {
	printf("TOKEN: %s at %dx%d\n", TokenSymbolStr[token.symbol], token.line, token.columnStart);
	printf("value: %s\n\n", token.value);
}

void debugTokens(TToken* tokens) {
	int i = 0;

	do {
		printToken(tokens[i]);
		i++;
	} while (tokens[i].symbol != FILE_END);
	printToken(tokens[i]);
}

void addTokenToFile(TToken token, FILE* fp) {
	fputs(TokenSymbolStr[token.symbol], fp);
	fputs(":", fp);
	fprintf(fp, "%d", token.line);
	fputs("x", fp);
	fprintf(fp, "%d", token.columnStart);
	fputs("\n", fp);
	fputs(token.value, fp);
	fputs("\n", fp);
}

void saveTokensDataToFile(TToken* tokens, char* filePath) {
	FILE *fp = fopen(filePath, "wb");

	if (fp != NULL)
	{
		int i = 0;
		do {
			addTokenToFile(tokens[i], fp);
			i++;
		} while (tokens[i].symbol != FILE_END);
		addTokenToFile(tokens[i], fp);

		fclose(fp);
	}
}

int lexer_main(int argc, char *argv[]) {
	TToken* tokens = readTokensFromFile(argv[1]);

	debugTokens(tokens);
	saveTokensDataToFile(tokens, "./out/tokens.txt");

  return 0;
}