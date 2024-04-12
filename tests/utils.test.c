#ifndef UTILS_T
#define UTILS_T

#include "./__utils.test.c"
#include "../src/utils/utils.c"

void readFileContentTest() {
  printf("#readFileContentTest\n");

  char* buffer = malloc(0);
  readFileContent("./tests/resources/__test_string.txt", buffer);
  assert(strcmp(buffer, "Hello World\n") == 0, "Should create string from file content");

  printf("\n");
}

int main(int argc, char const *argv[]) {
  readFileContentTest();

  return 0;
}

#endif