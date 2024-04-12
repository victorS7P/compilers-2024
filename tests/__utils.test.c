#ifndef __UTILS_T
#define __UTILS_T

#include "../src/config/config.c"

void assert(int assert, char* message, ...) {
  va_list args;
  va_start(args, message);

  if (assert) printf("✅ ");
  else printf("❌ ");

  vprintf(message, args);
  printf("\n");
  va_end(args);
}

int randomNumber (int min, int max) {
  srand(time(NULL));
  return rand()%((max+1)-min) + min;
}

#endif
