#include <stdio.h>
#include <inttypes.h>

#include "lib/returnExternArray.h"

// Test of calling an exported function that returns an array
int main(int argc, char* argv[]) {
  // Initialize the Chapel runtime and standard modules
  chpl_library_init(argc, argv);
  chpl__init_returnExternArray(1, 2);

  // Call the function to get the array
  chpl_external_array arr = foo();
  int64_t* actual = (int64_t*)arr.elts;

  // Write its elements
  for (int i = 0; i < 4; i++) {
    printf("Element[%d] = %" PRId64 "\n", i, actual[i]);
  }

  // Call the cleanup function
  chpl_free_external_array(arr);

  // Shutdown the Chapel runtime and standard modules
  chpl_library_finalize();

  return 0;
}
