#include <stdio.h>

#include "lib/requireHeader.h"

// Test of calling an exported function involving require statements
int main(int argc, char* argv[]) {
  // Initialize the Chapel runtime and standard modules
  chpl_library_init(argc, argv);
  chpl__init_requireHeader(0, 0);

  R foo;
  foo.b = 5;
  printsArg(foo);

  chpl_library_finalize();
  return 0;
}
