#ifndef LEXER_H
#define LEXER_H

#include "./lexer.h"
#include "../../utils/utils.c"

void debugToken(TLexerToken token) {
	debug("TOKEN: %s at %dx%d\n", TokenSymbolStr[token.symbol], token.line, token.columnStart);
	debug("value: %s\n\n", token.value);
}

void debugTokens(TLexerToken* tokens) {
	int i = 0;

	do {
		debugToken(tokens[i]);
		i++;
	} while (tokens[i].symbol != FILE_END);

	debugToken(tokens[i]);
}

int TokenAllowMultipleValues(TLexerToken token) {
	if (
		token.symbol == BLANK ||
		token.symbol == UNKNOWN ||
		token.symbol == LITERAL_VALUE
	) return 1;

	return 0;
}

TLexerToken createTokenFromChar(char c) {
	TLexerToken token;

	switch (c) {
		case EOF:
			token.symbol = FILE_END;
			break;

		case '\n':
			token.symbol = NEW_LINE;
			break;

		case ' ':
		case '\r':
		case '\0':
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
			token.symbol = MUL_OP;
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

TLexerToken getTokensWithSameSymbol(char* buffer, int* currentCharIndex) {
	TLexerToken token = createTokenFromChar(buffer[(*currentCharIndex)]);
	token.value = malloc(sizeof(char));
	token.value[0] = buffer[(*currentCharIndex)];
	token.valueLen = 1;

	int currentCharsInToken = 1;
	char nextFileChar = buffer[++(*currentCharIndex)];
	TLexerToken nextToken = createTokenFromChar(nextFileChar);

	if (!TokenAllowMultipleValues(token))
		return token;

	while (nextToken.symbol == token.symbol) {
		currentCharsInToken++;
		token.value = realloc(token.value, sizeof(char) * (currentCharsInToken));
		token.value[currentCharsInToken - 1] = nextFileChar;
		token.valueLen++;

		nextFileChar = buffer[++(*currentCharIndex)];
		nextToken = createTokenFromChar(nextFileChar);
	}

	return token;
}

TLexerToken* fetchTokensFromBuffer (char* buffer) {
	int bufferLen = strlen(buffer);
	int currentCharIndex = 0;

	int currentTokenIndex = 0;
	TLexerToken* tokens = malloc(currentTokenIndex);

	int currentBufferLine = 1;
	int currentBufferColumn = 1;

	while (currentCharIndex < bufferLen) {
		TLexerToken token = getTokensWithSameSymbol(buffer, &currentCharIndex);
		token.line = currentBufferLine;
		token.columnStart = currentBufferColumn;

		if (token.symbol == NEW_LINE) {
			currentBufferLine++;
			currentBufferColumn = 1;
			continue;
		}

		currentBufferColumn += token.valueLen;
		if (token.symbol == BLANK)
			continue;

		tokens = realloc(tokens, sizeof(TLexerToken) * (++currentTokenIndex));
		tokens[currentTokenIndex - 1] = token;
	}

	TLexerToken fileEndToken = {
		symbol: FILE_END,
		value: NULL,
		line: currentBufferLine,
		columnStart: currentBufferColumn
	};

	tokens = realloc(tokens, sizeof(TLexerToken) * (++currentTokenIndex));
	tokens[currentTokenIndex - 1] = fileEndToken;

	return tokens;
}

int addTokenToBuffer(TLexerToken token, char** buffer, int bufferLen) {
	int tempSize = (sizeof(char) * 1024);
	char bufferToAppend[tempSize];

	int bufferToAppendLen = sprintf(bufferToAppend, "%s:", TokenSymbolStr[token.symbol]);
	bufferToAppendLen += sprintf(bufferToAppend+bufferToAppendLen, " %03dx", token.line);
	bufferToAppendLen += sprintf(bufferToAppend+bufferToAppendLen, "%03d\n", token.columnStart);
	bufferToAppendLen += sprintf(bufferToAppend+bufferToAppendLen, "%s\n\n", token.value);

	(*buffer) = realloc((*buffer), (bufferLen + bufferToAppendLen));
	return sprintf((*buffer)+bufferLen, "%s", bufferToAppend);
}

char* createBufferWithTokensData(TLexerToken* tokens) {
	int currentLen = 0;
	char* buffer = malloc(currentLen);

	int bufferLen = 0;
	int currentTokenIndex = 0;
	do {
		bufferLen += addTokenToBuffer(tokens[currentTokenIndex++], &buffer, bufferLen);
	}	while (tokens[currentTokenIndex].symbol != FILE_END);

	addTokenToBuffer(tokens[currentTokenIndex], &buffer, bufferLen);
	return buffer;
}

#endif
