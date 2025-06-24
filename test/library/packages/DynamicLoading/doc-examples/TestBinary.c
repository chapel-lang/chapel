//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//

/* START_EXAMPLE */
// Compile with: 'cc -shared -fPIC -o TestBinary TestBinary.c'
//
// Your C compiler must support compiling a shared library as well
// as generating "position independent code". If your compiler
// offers some other way of preserving symbol tables (e.g., the
// '-rdynamic' flag) for use with dynamic loading, you can compile
// a regular executable with that flag as well.
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
/* STOP_EXAMPLE */
