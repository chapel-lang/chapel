#include <stdio.h>

#include "lib/headerName.h"

// Test of calling an exported function that takes an array
int main(int argc, char* argv[]) {
  // Initialize the Chapel runtime and standard modules
  chpl_library_init(argc, argv);
  chpl__init_renamedLib(0, 0);

  foo();

  chpl_library_finalize();
  return 0;
}
