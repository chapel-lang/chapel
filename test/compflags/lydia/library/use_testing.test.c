#include "lib/testing.h"

extern void chpl_library_init(int argc, char* argv[]);
extern void chpl_library_finalize(void);

// Test of calling an exported Chapel library.
int main(int argc, char* argv[]) {
  // Initialize the Chapel runtime and standard modules
  chpl_library_init(argc, argv);

  // Call the function
  foo();

  // Shutdown the Chapel runtime and standard modules
  chpl_library_finalize();

  return 0;
}
