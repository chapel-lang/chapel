#include <stdio.h>

int addTwoReturn(int a, int b);
int addTwoReturn(int a, int b) { return a + b; }

int main(int argc, char** argv) {
  int x = addTwoReturn(2, 2);
  printf("%d\n", x);
  return 0;
}
