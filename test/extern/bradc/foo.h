typedef int foo;

void printfoo(foo x);
void zerofoo(foo* x);

void zerofoo(foo* x) {
  *x = 0;
}

void printfoo(foo x) {
  printf("%d\n", x);
}
