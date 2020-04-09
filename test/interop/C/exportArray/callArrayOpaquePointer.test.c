#include <inttypes.h>

#include "lib/arrayOpaquePointer.h"

// Test of calling an exported function that takes an array
int main(int argc, char* argv[]) {
  // Initialize the Chapel runtime and standard modules
  chpl_library_init(argc, argv);
  chpl__init_arrayOpaquePointer(0, 0);

  chpl_opaque_array arr = makeBlockArray();
  printBlock(&arr);
  addEltBlock(&arr, 2, 3);
  printBlock(&arr);

  // Call cleanup function when done with it
  cleanupOpaqueArray(&arr);

  chpl_library_finalize();
  return 0;
}
