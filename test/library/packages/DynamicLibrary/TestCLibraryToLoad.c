#include <stdio.h>

int addTwoReturn(int a, int b);
int addTwoReturn(int a, int b) { return a + b; }

int num = 64;

void foo(void);
void foo(void) {
  printf("Hello world from %s!\n", __FUNCTION__);
}

int main(int argc, char** argv) {
  int x = addTwoReturn(2, 2);
  printf("%d\n", x);
  foo();
  (void) num;
  return 0;
}
