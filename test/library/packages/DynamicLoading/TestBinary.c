// Compile with: 'cc -o binary binary.c'
//

#include <stdio.h>

void foo(void);
void foo(void) {
  printf("Hello world from %s!\n", __FUNCTION__);
}

int main(void) {
  foo();
  return 0;
}
