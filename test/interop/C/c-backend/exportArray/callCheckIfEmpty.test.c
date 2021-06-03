#include "lib/checkIfEmpty.h"

// Test of calling an exported function that takes an empty array
int main(int argc, char* argv[]) {
  // Initialize the Chapel runtime and standard modules
  chpl_library_init(argc, argv);
  chpl__init_checkIfEmpty(0, 0);

  // Call the function
  chpl_external_array emptyArr = chpl_make_external_array_ptr(NULL, 0);
  printIfEmptyArr(&emptyArr);

  // Shutdown the Chapel runtime and standard modules
  chpl_library_finalize();
  return 0;
}
