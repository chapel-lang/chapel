#include <stdio.h>
#include <inttypes.h>

#include "lib/givenName.h"

extern void chpl_library_init(int argc, char* argv[]);
extern void chpl_library_finalize(void);

// Test of calling an exported function that returns an array
int main(int argc, char* argv[]) {
  // Initialize the Chapel runtime and standard modules
  chpl_library_init(argc, argv);
  chpl__init_A(1, 2);
  chpl__init_B(1, 2);

  // Call the functions
  bar();
  baz();

  // Shutdown the Chapel runtime and standard modules
  chpl_library_finalize();

  return 0;
}
