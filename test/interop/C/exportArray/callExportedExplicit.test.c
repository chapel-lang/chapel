#include <stdio.h>
#include <inttypes.h>

#include "lib/exportFuncWithExplicitVoidRet.h"

// Test of calling an exported function that takes an array
int main(int argc, char* argv[]) {
  // Initialize the Chapel runtime and standard modules
  chpl_library_init(argc, argv);
  chpl__init_exportFuncWithExplicitVoidRet(0, 0);

  // Call the function
  int64_t x[5] = {1, 2, 3, 4, 5};
  chpl_external_array arrX = chpl_make_external_array_ptr(x, 5);
  foo(&arrX);
  for (int i = 0; i < 5; i++) {
    printf("Element[%d] = %" PRId64 "\n", i, x[i]);
  }

  // Shutdown the Chapel runtime and standard modules
  chpl_library_finalize();

  return 0;
}
