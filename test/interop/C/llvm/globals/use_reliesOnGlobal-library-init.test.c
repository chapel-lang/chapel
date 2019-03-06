#include "lib/reliesOnGlobal.h"

// Test of calling an exported Chapel library that relies on a global.
int main(int argc, char* argv[]) {
  // Initialize the Chapel runtime and standard modules
  chpl_library_init(argc, argv);

  chpl__init_reliesOnGlobal(1, 2);

  // Call the function
  whatIsGlobal();

  // Shutdown the Chapel runtime and standard modules
  chpl_library_finalize();

  return 0;
}
