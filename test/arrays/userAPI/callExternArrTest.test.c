#include <stdio.h>
#include <inttypes.h>
#include "lib/externArrTest.h"
// Test of calling an exported function that takes an array
int main(int argc, char* argv[]) {
  // Initialize the Chapel runtime and standard modules
  chpl_library_init(argc, argv);
  chpl__init_externArrTest(0, 0);
  // Call the function
  double x[5] = {1.0, 2.0, 3.0, 4.0, 5.0};
  chpl_external_array arrX = chpl_make_external_array_ptr(x, 5);
  checkExportedArray(&arrX);
  // Shutdown the Chapel runtime and standard modules
  chpl_library_finalize();
  return 0;
}
