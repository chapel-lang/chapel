#include <stdio.h>
#include "lib/foo.h"

// Test of calling an exported Chapel library.
int main(int argc, char* argv[]) {
  // Initialize the Chapel runtime and standard modules twice This is
  // not legal/supported, so the second should result in an error.
  chpl_library_init(argc, argv);

  // Call the function
  foo();

  // Shutdown the Chapel runtime and standard modules
  chpl_library_finalize();
  printf("After chpl_library_finalize()\n");

  return 0;
}
