#include <stdbool.h>
#include <stdio.h>

#include "lib/returnArrayOfReals.h"

// Test of calling an exported function that returns an array
int main(int argc, char* argv[]) {
  // Initialize the Chapel runtime and standard modules
  chpl_library_init(argc, argv);
  chpl__init_returnArrayOfReals(1, 2);

  // Call the function to get the array
  chpl_external_array arr = foo();
  double* actual = (double*)arr.elts;

  // Write its elements
  for (int i = 0; i < 4; i++) {
    bool worked = (actual[i] <= i + 2) && (actual[i] > i);
    printf("Element[%d] %s\n", i, worked ? "worked" : "did not work");
  }

  // Call the cleanup function
  chpl_free_external_array(arr);

  // Shutdown the Chapel runtime and standard modules
  chpl_library_finalize();

  return 0;
}
