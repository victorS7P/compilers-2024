#ifndef PARSER_T
#define PARSER_T

#include "./__utils.test.c"
#include "../src/modules/parser/parser.c"

#include "../src/modules/vm/vm.c"
#include "../src/modules/assembler/assembler.c"

void parseExpressionTest() {
  printf("#parseExpression\n");

  int currentTokenIndex = 0; int result = 0;
  TParserTree* tree = initTree();
  TParserAssemblerData* assemblerData = initAssemblerData();

  TLexerToken* emptyOperationTokens = fetchTokensFromBuffer("");
  result = parseExpression(tree, emptyOperationTokens, &currentTokenIndex, assemblerData);
  assert(
    result == 0 &&
    tree[0].symbol == BNF_EXPRESSION &&
    tree[0].childrenLength == 0
  , "Should properly parse an empty operation");
  assert(
    assemblerData->literalValuesLength == 0 &&
    assemblerData->operationsLength == 0
  , "Should properly parse assembler data for an empty operation");

  currentTokenIndex = 0;
  assemblerData = initAssemblerData();
  TLexerToken* justNumberTokens = fetchTokensFromBuffer("32");
  result = parseExpression(tree, justNumberTokens, &currentTokenIndex, assemblerData);
  assert(
    result == 1 &&
    tree[0].symbol == BNF_EXPRESSION &&
    tree[0].childrenLength == 2 &&
    tree[0].childrenTree[0].symbol == BNF_LITERAL_VALUE &&
    tree[0].childrenTree[0].token.symbol == LITERAL_VALUE &&
    tree[0].childrenTree[1].symbol == BNF_FILE_END &&
    tree[0].childrenTree[1].token.symbol == FILE_END
  , "Should properly parse a number-only token");
  assert(
    strcmp(tree[0].childrenTree[0].token.value, "32") == 0,
    "Should properly parse a number-only token value"
  );
  assert(
    assemblerData->literalValuesLength == 1 &&
    assemblerData->literalValues[0] == 32 &&
    assemblerData->operationsLength == 1 &&
    assemblerData->operations[0] == FILE_END
  , "Should properly parse assembler data for a number-only operaiton");

  currentTokenIndex = 0;
  assemblerData = initAssemblerData();
  TLexerToken* simpleOperationToken = fetchTokensFromBuffer("13+21");
  result = parseExpression(tree, simpleOperationToken, &currentTokenIndex, assemblerData);
  assert(
    result == 1 &&
    tree[0].symbol == BNF_EXPRESSION &&
    tree[0].childrenLength == 3 &&
    tree[0].childrenTree[0].symbol == BNF_LITERAL_VALUE &&
    tree[0].childrenTree[1].symbol == BNF_OPERATOR &&
    tree[0].childrenTree[2].symbol == BNF_EXPRESSION &&
    tree[0].childrenTree[2].childrenLength == 2 &&
    tree[0].childrenTree[2].childrenTree[0].symbol == BNF_LITERAL_VALUE &&
    tree[0].childrenTree[2].childrenTree[1].symbol == BNF_FILE_END
  , "Should properly parse a simple operation token");
  assert(
    strcmp(tree[0].childrenTree[0].token.value, "13") == 0 &&
    strcmp(tree[0].childrenTree[2].childrenTree[0].token.value, "21") == 0
  , "Should properly parse a simple operation token values");
  assert(
    assemblerData->literalValuesLength == 2 &&
    assemblerData->literalValues[0] == 13 &&
    assemblerData->literalValues[1] == 21 &&
    assemblerData->operationsLength == 2 &&
    assemblerData->operations[0] == SUM_OP &&
    assemblerData->operations[1] == FILE_END
  , "Should properly parse assembler data for a simple operaiton");

  currentTokenIndex = 0;
  assemblerData = initAssemblerData();
  TLexerToken* nextedOperationToken = fetchTokensFromBuffer("1/2*3-4");
  result = parseExpression(tree, nextedOperationToken, &currentTokenIndex, assemblerData);
  assert(
    result == 1 &&
    tree[0].symbol == BNF_EXPRESSION &&
    tree[0].childrenLength == 3 &&
    tree[0].childrenTree[0].symbol == BNF_LITERAL_VALUE &&
    tree[0].childrenTree[1].symbol == BNF_OPERATOR &&
    tree[0].childrenTree[2].symbol == BNF_EXPRESSION &&
    tree[0].childrenTree[2].childrenLength == 3 &&
    tree[0].childrenTree[2].childrenTree[0].symbol == BNF_LITERAL_VALUE &&
    tree[0].childrenTree[2].childrenTree[1].symbol == BNF_OPERATOR &&
    tree[0].childrenTree[2].childrenTree[2].symbol == BNF_EXPRESSION
  , "Should properly parse a nested operation token");
  assert(
    strcmp(tree[0].childrenTree[0].token.value, "1") == 0 &&
    strcmp(tree[0].childrenTree[2].childrenTree[0].token.value, "2") == 0 &&
    strcmp(tree[0].childrenTree[2].childrenTree[2].childrenTree[0].token.value, "3") == 0
  , "Should properly parse a nested operation token values");
  assert(
    assemblerData->literalValuesLength == 4 &&
    assemblerData->literalValues[0] == 1 &&
    assemblerData->literalValues[1] == 2 &&
    assemblerData->literalValues[2] == 3 &&
    assemblerData->operationsLength == 4 &&
    assemblerData->operations[0] == DIV_OP &&
    assemblerData->operations[1] == MUL_OP &&
    assemblerData->operations[3] == FILE_END
  , "Should properly parse assembler data for anested operaiton");

  free(tree);
  free(assemblerData);
  printf("\n");
}

int main(int argc, char const *argv[]) {
  parseExpressionTest();
  return 0;
}

#endif