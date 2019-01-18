#include "lib/defaultValueWarning.h"

// Test of calling an exported Chapel library.
int main(int argc, char* argv[]) {
  // Initialize the Chapel runtime and standard modules
  chpl_library_init(argc, argv);

  // Call the function
  foo(3);

  // Shutdown the Chapel runtime and standard modules
  chpl_library_finalize();

  return 0;
}
