#include "./utils/utils.c";

#include "./modules/vm/vm.c";
#include "./modules/assembler/assembler.c";
#include "./modules/lexer/lexer.c";
#include "./modules/parser/parser.c";

#define TOKENS_DEBUG_FILE "./debug/01-tokens.debug.txt"
#define PARSER_TREE_DEBUG_FILE "./debug/02-parser-tree.debug.txt"
#define ASSEMBLY_DEBUG_FILE "./debug/03-assembly.debug.txt"
#define VM_START_MEMORY_DEBUG_FILE "./debug/04-vm-start-memory.debug.txt"
#define VM_FINAL_MEMORY_DEBUG_FILE "./debug/05-vm-final-memory.debug.txt"
#define VM_FINAL_STATE_DEBUG_FILE "./debug/06-vm-final-state.debug.txt"

TLexerToken* createAndSaveTokensDebugFromExpression(char* buffer) {
  TLexerToken* tokens = fetchTokensFromBuffer(buffer);

  char* tokensBuffer = createBufferWithTokensData(tokens);
  writeContentToFile(TOKENS_DEBUG_FILE, tokensBuffer);
  free(tokensBuffer);

  return tokens;
}

char* createAndSaveParserAndAssemblyFromTokens(TLexerToken* tokens) {
  TParserTree* parserTree = initTree();
	TParserAssemblerData* assemblerData = initAssemblerData();
  int isValidExpression = fetchParserDataFromTokens(parserTree, assemblerData, tokens);

  if (isValidExpression == 0) {
    free(parserTree);
    free(assemblerData);

    printf("ERRO!\nExpressão inválida!\n");
    exit(1);
  }

  char* parserTreeBuffer = createBufferWithParserTreeData(parserTree);
  writeContentToFile(PARSER_TREE_DEBUG_FILE, parserTreeBuffer);
  free(parserTree);
  free(parserTreeBuffer);

  char* assemblyBuffer = createAssemblyFromData(assemblerData);
  writeContentToFile(ASSEMBLY_DEBUG_FILE, parserTreeBuffer);

  free(assemblerData);
  return assemblyBuffer;
}

char* createAndSaveVmMemoryFromAssemblyBuffer(char* assemblyBuffer) {
  int* vmMemory = createMemoryFromAssemblyBuffer(assemblyBuffer);
  char* vmMemoryBuffer = saveMemoryToBuffer(vmMemory);
  writeContentToFile(VM_START_MEMORY_DEBUG_FILE, vmMemoryBuffer);
  free(vmMemory);

  return vmMemoryBuffer;
}

TNeanderVM createAndExecuteNeanderVmFromMemoryBuffer(char* memoryBuffer) {
  TNeanderVM vm = createNeanderVM();
  setNeanderVMFromStringBuffer(&vm, memoryBuffer);

  executeVM(&vm);
  return vm;
}

void saveAndPrintFinalVmResults(TNeanderVM* vm, char* expression) {
  char* vmFinalMemoryBuffer = saveMemoryToBuffer(vm->memory);
  writeContentToFile(VM_FINAL_MEMORY_DEBUG_FILE, vmFinalMemoryBuffer);
  free(vmFinalMemoryBuffer);

  char* vmFinalStateBuffer = createBufferFromVmData(vm);
  writeContentToFile(VM_FINAL_STATE_DEBUG_FILE, vmFinalStateBuffer);

  printf("Expression \"%s\" evaluated!\n", expression);
  printf("Result = %d\n\n", vm->accumulator);
  printf("=== FINAL VM STATE ===\n%s\n", vmFinalStateBuffer);
  printf("❕ Check /debug folder to see files result for each module\n");

  free(vmFinalStateBuffer);
}

int main () {
  char* expression = "100-10-20-30-40-50";
  TLexerToken* tokens = createAndSaveTokensDebugFromExpression(expression);

  char* assemblyBuffer = createAndSaveParserAndAssemblyFromTokens(tokens);
  free(tokens);

  char* vmMemoryBuffer = createAndSaveVmMemoryFromAssemblyBuffer(assemblyBuffer);
  free(assemblyBuffer);

  TNeanderVM vm = createAndExecuteNeanderVmFromMemoryBuffer(vmMemoryBuffer);
  free(vmMemoryBuffer);

  saveAndPrintFinalVmResults(&vm, expression);
  return 0;
}
