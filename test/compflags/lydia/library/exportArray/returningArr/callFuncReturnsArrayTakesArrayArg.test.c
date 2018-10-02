#include <stdio.h>
#include <inttypes.h>

#include "lib/returnArrayTakesArrayArg.h"

// Test of calling an exported function that returns an array
int main(int argc, char* argv[]) {
  // Initialize the Chapel runtime and standard modules
  chpl_library_init(argc, argv);
  chpl__init_returnArrayTakesArrayArg(1, 2);

  int64_t x[4] = {1, 2, 3, 4};
  chpl_external_array arg = chpl_make_external_array_ptr(x, 4);
  // Call the function to get the array
  chpl_external_array arr = foo(&arg);
  int64_t* actual = (int64_t*)arr.elts;

  if (arr.size != arg.size) {
    printf("Uh oh!");
  }

  // Write its elements
  for (int i = 0; i < arr.size; i++) {
    printf("Element[%d] = %" PRId64 "\n", i, actual[i]);
  }

  // Call the cleanup function
  chpl_free_external_array(arr);

  // Shutdown the Chapel runtime and standard modules
  chpl_library_finalize();

  return 0;
}
