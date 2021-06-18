#include "lib/hyphenated-name.h"

// Test of calling an exported Chapel library.
int main(int argc, char* argv[]) {
  // Initialize the Chapel runtime and standard modules
  chpl_library_init(argc, argv);

  // TODO (dlongnecke): What should the module initializer function be
  // named in this situation?

  // Call the function
  foo();

  // Shutdown the Chapel runtime and standard modules
  chpl_library_finalize();

  return 0;
}
