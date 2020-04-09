#include <stdio.h>
#include <inttypes.h>

#include "mydir/setDir.h"

// Test of calling an exported function that returns an array
int main(int argc, char* argv[]) {
  // Initialize the Chapel runtime and standard modules
  chpl_library_init(argc, argv);
  chpl__init_setDir(1, 2);

  // Call the function
  whatever();

  // Shutdown the Chapel runtime and standard modules
  chpl_library_finalize();

  return 0;
}
