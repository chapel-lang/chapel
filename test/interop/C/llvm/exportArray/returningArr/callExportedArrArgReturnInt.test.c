#include <stdio.h>
#include <inttypes.h>

#include "lib/exportArrArgReturnInt.h"

// Test of calling an exported function that takes an array
int main(int argc, char* argv[]) {
  // Initialize the Chapel runtime and standard modules
  chpl_library_init(argc, argv);
  chpl__init_exportArrArgReturnInt(0, 0);

  int64_t x[5] = {1, 2, 3, 4, 5};
  chpl_external_array arrX = chpl_make_external_array_ptr(x, 5);
  int64_t count = numEven(&arrX);
  printf("%" PRId64 "\n", count);

  // Shutdown the Chapel runtime and standard modules
  chpl_library_finalize();

  return 0;
}
