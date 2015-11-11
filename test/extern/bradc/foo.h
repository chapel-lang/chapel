typedef int foo;

void printfoo(foo x);

void printfoo(foo x) {
  x = 0;
  printf("%d\n", x);
}
