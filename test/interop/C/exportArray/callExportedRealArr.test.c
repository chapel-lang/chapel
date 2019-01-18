#include <stdbool.h>
#include <stdio.h>

#include "lib/exportFuncWithRealArr.h"

// Test of calling an exported function that takes an array
int main(int argc, char* argv[]) {
  // Initialize the Chapel runtime and standard modules
  chpl_library_init(argc, argv);

  // Call the function
  double x[5] = {1.0, 2.0, 3.0, 4.0, 5.0};
  chpl_external_array arrX = chpl_make_external_array_ptr(x, 5);
  foo(&arrX);
  for (int i = 0; i < 5; i++) {
    bool worked = (x[i] <= i + 3) && (x[i] > i + 1);
    printf("Element[%d] %s\n", i, worked ? "worked" : "did not work");
  }

  // Shutdown the Chapel runtime and standard modules
  chpl_library_finalize();

  return 0;
}
