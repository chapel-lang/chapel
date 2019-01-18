#include <inttypes.h>

#include "lib/arrayOpaquePointer_noArgDom.h"

// Test of calling an exported function that takes an array
int main(int argc, char* argv[]) {
  // Initialize the Chapel runtime and standard modules
  chpl_library_init(argc, argv);
  chpl__init_arrayOpaquePointer_noArgDom(0, 0);

  chpl_opaque_array arr = makeBlockArray();
  printBlock(&arr); // Expected to fail

  chpl_library_finalize();
  return 0;
}
