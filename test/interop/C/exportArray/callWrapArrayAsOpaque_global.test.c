#include <inttypes.h>

#include "lib/wrapArrayAsOpaque_global.h"

// Test of calling an exported function that takes an array
int main(int argc, char* argv[]) {
  // Initialize the Chapel runtime and standard modules
  chpl_library_init(argc, argv);
  chpl__init_wrapArrayAsOpaque_global(0, 0);

  chpl_opaque_array arr = makeBlockArray();
  printBlock(&arr);
  addEltBlock(&arr, 2, 3);
  printBlock(&arr);

  chpl_opaque_array arr2 = makeBlockArray();
  printBlock(&arr2);

  chpl_opaque_array arr3 = makeBlockArrayRef();
  printBlock(&arr3);
  addEltBlock(&arr3, 2, 3);
  printBlock(&arr3);

  chpl_opaque_array arr4 = makeBlockArray();
  printBlock(&arr4);

  // Call clean up function when exported (on how many?)
  //chpl_mem_free(arr, 0, 0);
  //chpl_mem_free(arr2, 0, 0);
  //chpl_mem_free(arr3, 0, 0);
  //chpl_mem_free(arr4, 0, 0);

  chpl_library_finalize();
  return 0;
}
