#include <stdio.h>

#include "lib/myLib.h"

// Test of calling an exported function that returns an array
int main(int argc, char* argv[]) {
  // Initialize the Chapel runtime and standard modules
  chpl_library_init(argc, argv);
  chpl__init_myLib(1, 2);

  puts("What is it good for?");

  const char *res = whatisItGoodFor();
  puts(res);

  chpl_library_finalize();
}
