#ifndef LEXER_T
#define LEXER_T

#include "./__utils.test.c"
#include "../src/modules/lexer/lexer.c"

void createTokenFromCharTest() {
  printf("#createTokenFromChar\n");

  assert(createTokenFromChar('\n').symbol == NEW_LINE, "Should return NEW_LINE token");

  assert(createTokenFromChar(' ').symbol == BLANK, "Should return BLANK token");
  assert(createTokenFromChar('\r').symbol == BLANK, "Should return BLANK token");
  assert(createTokenFromChar('\t').symbol == BLANK, "Should return BLANK token");
  assert(createTokenFromChar('\0').symbol == BLANK, "Should return BLANK token");

  assert(createTokenFromChar('(').symbol == PARENTHESIS_OPEN, "Should return PARENTHESIS_OPEN token");
  assert(createTokenFromChar(')').symbol == PARENTHESIS_CLOSE, "Should return PARENTHESIS_CLOSE token");

  assert(createTokenFromChar('+').symbol == SUM_OP, "Should return SUM_OP token");
  assert(createTokenFromChar('-').symbol == SUB_OP, "Should return SUB_OP token");
  assert(createTokenFromChar('*').symbol == MUL_OP, "Should return MUL_OP token");
  assert(createTokenFromChar('/').symbol == DIV_OP, "Should return DIV_OP token");

  assert(createTokenFromChar('0').symbol == LITERAL_VALUE, "Should return LITERAL_VALUE token");
  assert(createTokenFromChar('5').symbol == LITERAL_VALUE, "Should return LITERAL_VALUE token");
  assert(createTokenFromChar('9').symbol == LITERAL_VALUE, "Should return LITERAL_VALUE token");

  assert(createTokenFromChar('a').symbol == UNKNOWN, "Should return UNKNOWN token");
  assert(createTokenFromChar('B').symbol == UNKNOWN, "Should return UNKNOWN token");
  assert(createTokenFromChar('#').symbol == UNKNOWN, "Should return UNKNOWN token");

  printf("\n");
}

void getTokensWithSameSymbolTest() {
  printf("#getTokensWithSameSymbol\n");

  int currentCharIndex = 0;
  TLexerToken result = getTokensWithSameSymbol("123\n", &currentCharIndex);
  assert(strcmp(result.value, "123") == 0, "Should get a chain of literals as a single token");
  assert(result.symbol == LITERAL_VALUE, "Should get the correct symbol for a chain of literals");
  assert(result.valueLen == 3, "Should properly set valueLen");
  assert(currentCharIndex == 3, "Should update currentCharIndex");

  currentCharIndex = 0;
  result = getTokensWithSameSymbol("41+13\n", &currentCharIndex);
  assert(strcmp(result.value, "41") == 0, "Should break a chain of literal with other symbol");
  assert(result.symbol == LITERAL_VALUE, "Should get the correct symbol for a chain of literals");
  assert(result.valueLen == 2, "Should properly set valueLen");
  assert(currentCharIndex == 2, "Should update currentCharIndex");

  currentCharIndex = 0;
  result = getTokensWithSameSymbol("(1+2)\n", &currentCharIndex);
  assert(strcmp(result.value, "(") == 0, "Should get a single token when second is different");
  assert(result.symbol == PARENTHESIS_OPEN, "Should get the correct symbol for a single token");
  assert(result.valueLen == 1, "Should properly set valueLen");
  assert(currentCharIndex == 1, "Should update currentCharIndex");

  currentCharIndex = 0;
  result = getTokensWithSameSymbol("((1+2)/3)\n", &currentCharIndex);
  assert(strcmp(result.value, "(") == 0, "Should get a single token when it doesnt allow multiple values");
  assert(result.symbol == PARENTHESIS_OPEN, "Should get the correct symbol for a single token");
  assert(result.valueLen == 1, "Should properly set valueLen");
  assert(currentCharIndex == 1, "Should update currentCharIndex");

  currentCharIndex = 0;
  result = getTokensWithSameSymbol("a#!va3\n", &currentCharIndex);
  assert(strcmp(result.value, "a#!va") == 0, "Should get a chain of unknown chars");
  assert(result.symbol == UNKNOWN, "Should get the correct symbol for a chain of unknown");
  assert(result.valueLen == 5, "Should properly set valueLen");
  assert(currentCharIndex == 5, "Should update currentCharIndex");

  currentCharIndex = 0;
  result = getTokensWithSameSymbol("  \t\t \r  4*2\n", &currentCharIndex);
  assert(strcmp(result.value, "  \t\t \r  ") == 0, "Should get a chain of blank chars");
  assert(result.symbol == BLANK, "Should get the correct symbol for a chain of blank");
  assert(result.valueLen == 8, "Should properly set valueLen");
  assert(currentCharIndex == 8, "Should update currentCharIndex");

  currentCharIndex = 5;
  result = getTokensWithSameSymbol("12+3a45/5", &currentCharIndex);
  assert(strcmp(result.value, "45") == 0, "Should properly get token when currentCharIndex is set");
  assert(result.symbol == LITERAL_VALUE, "Should get the correct symbol for a token that is not at buffer start");
  assert(result.valueLen == 2, "Should properly set valueLen");
  assert(currentCharIndex == 7, "Should update currentCharIndex");

  printf("\n");
}

void fetchTokensFromBufferTest() {
  printf("#fetchTokensFromBuffer\n");

  TLexerToken* tokens = fetchTokensFromBuffer("(1+2)");
  assert(
    strcmp(tokens[0].value, "(") == 0 &&
    strcmp(tokens[1].value, "1") == 0 &&
    strcmp(tokens[2].value, "+") == 0 &&
    strcmp(tokens[3].value, "2") == 0 &&
    strcmp(tokens[4].value, ")") == 0
  , "Should properly parse 1+2 tokens values");
  assert(
    tokens[0].symbol == PARENTHESIS_OPEN  &&
    tokens[1].symbol == LITERAL_VALUE     &&
    tokens[2].symbol == SUM_OP            &&
    tokens[3].symbol == LITERAL_VALUE     &&
    tokens[4].symbol == PARENTHESIS_CLOSE &&
    tokens[5].symbol == FILE_END
  , "Should properly parse 1+2 tokens symbols");
  assert(
    tokens[0].line == 1 &&
    tokens[1].line == 1 &&
    tokens[2].line == 1 &&
    tokens[3].line == 1 &&
    tokens[4].line == 1 &&
    tokens[5].line == 1
  , "Should properly parse 1+2 tokens line");
  assert(
    tokens[0].columnStart == 1 &&
    tokens[1].columnStart == 2 &&
    tokens[2].columnStart == 3 &&
    tokens[3].columnStart == 4 &&
    tokens[4].columnStart == 5 &&
    tokens[5].columnStart == 6
  , "Should properly parse 1+2 tokens column start");
  assert(
    tokens[0].valueLen == 1 &&
    tokens[1].valueLen == 1 &&
    tokens[2].valueLen == 1 &&
    tokens[3].valueLen == 1 &&
    tokens[4].valueLen == 1
  , "Should properly parse 1+2 tokens valueLen");

  char* buffer = malloc(0);
  readFileContent("./tests/resources/lexer_input.txt", &buffer);
  tokens = fetchTokensFromBuffer(buffer);
  assert(
    strcmp(tokens[0].value,  "500") == 0 &&
    strcmp(tokens[1].value,  "a")   == 0 &&
    strcmp(tokens[2].value,  "500") == 0 &&
    strcmp(tokens[5].value,  "(")   == 0 &&
    strcmp(tokens[14].value, "abc") == 0
  , "Should properly parse tokens values from file");
  assert(
    tokens[0].symbol  == LITERAL_VALUE    &&
    tokens[1].symbol  == UNKNOWN          &&
    tokens[2].symbol  == LITERAL_VALUE    &&
    tokens[5].symbol  == PARENTHESIS_OPEN &&
    tokens[14].symbol == UNKNOWN
  , "Should properly parse tokens symbols from file");
  assert(
    tokens[0].line   == 1 &&
    tokens[1].line   == 1 &&
    tokens[6].line   == 2 &&
    tokens[14].line  == 4 &&
    tokens[15].line  == 5 &&
    tokens[18].line  == 7
  , "Should properly parse tokens line from file");
  assert(
    tokens[0].columnStart   == 7 &&
    tokens[1].columnStart   == 10 &&
    tokens[6].columnStart   == 2 &&
    tokens[14].columnStart  == 5 &&
    tokens[15].columnStart  == 1 &&
    tokens[18].columnStart  == 1
  , "Should properly parse tokens column start from file");
  assert(
    tokens[0].valueLen   == 3 &&
    tokens[1].valueLen   == 1 &&
    tokens[6].valueLen   == 1 &&
    tokens[14].valueLen  == 3 &&
    tokens[15].valueLen  == 3
  , "Should properly parse tokens valueLen from file");

  free(tokens);
  free(buffer);
  printf("\n");
}

void createBufferWithTokensDataTest() {
  printf("#createBufferWithTokensData\n");

  TLexerToken tokens[] = {
    [0] = { 1, LITERAL_VALUE, "150", 3, 1 },
    [1] = { 1, DIV_OP       , "/"  , 1, 4 },
    [2] = { 1, LITERAL_VALUE, "50" , 2, 5 },
    [3] = { 2, FILE_END     , NULL , 0, 1 }
  };

  char* buffer = createBufferWithTokensData(tokens);
  assert(
    strstr(buffer, "LITERAL_VALUE: 001x001\n150\n\n") != NULL &&
    strstr(buffer, "DIV_OP: 001x004\n/\n\n") != NULL          &&
    strstr(buffer, "LITERAL_VALUE: 001x005\n50\n\n") != NULL  &&
    strstr(buffer, "FILE_END: 002x001\n") != NULL
  , "Should create a buffer from tokens data");

  free(buffer);
  printf("\n");
}

int main(int argc, char const *argv[]) {
  createTokenFromCharTest();
  getTokensWithSameSymbolTest();

  fetchTokensFromBufferTest();
  createBufferWithTokensDataTest();

  return 0;
}

#endif