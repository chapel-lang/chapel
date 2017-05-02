#include <stdio.h>

int foobar(int x) {
  return x * 2;
}

int main() {
  int x = 5;
  int y = foobar(x);

  printf("foobar(%d) = %d\n", x, y);

  printf("Hello world\n");

  return 0;
}
