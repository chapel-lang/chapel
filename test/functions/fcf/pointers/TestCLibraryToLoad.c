#include <stdio.h>
#include "TestTypeOnlyHeader.h"

int addTwoReturn(int a, int b);
void printFoobar(foobar f);
foobar createFoobar(double a, double b, double c,
                    double d,
                    double e,
                    double f,
                    double g,
                    double h,
                    baz x);

int addTwoReturn(int a, int b) { return a + b; }

void printFoobar(foobar f) {
  printf("{ %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, { %d, %d, %d, %d } }\n",
         f.a, f.b, f.c, f.d, f.e, f.f, f.g, f.h, f.x.a, f.x.b, f.x.c, f.x.d);
}

foobar createFoobar(double a, double b, double c,
                    double d,
                    double e,
                    double f,
                    double g,
                    double h,
                    baz x) {
  foobar ret = { a, b, c, d, e, f, g, h, x };
  for (int i = 0; i < FOOBAR_PADDING; i++) ret.padding[i] = i;
  printFoobar(ret);
  return ret;
}

int main(int argc, char** argv) {
  int x = addTwoReturn(2, 2);
  printf("%d\n", x);
  baz b = { 8, 9, 10, 11 };
  foobar y = createFoobar(0, 1, 2, 3, 4, 5, 6, 7, b);
  printFoobar(y);
  return 0;
}
