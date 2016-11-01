#include <mtbb/task_group.h>

long bin(int n) {
  if (n == 0) return 1;
  else {
    mtbb::task_group tg;
    long x, y;
    tg.run([=,&x] { x = bin(n - 1); });
    y = bin(n - 1);
    tg.wait();
    return x + y;
  }
}

int main(int argc, char ** argv) {
  int n = atoi(argv[1]);
  long x = bin(n);
  printf("bin(%d) = %ld\n", n, x);
  return 0;
}

