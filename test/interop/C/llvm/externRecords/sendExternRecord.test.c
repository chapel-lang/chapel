#include "lib/receiveExternRecord.h"

// Test of calling an exported function that takes an array
int main(int argc, char* argv[]) {
  // Initialize the Chapel runtime and standard modules
  chpl_library_init(argc, argv);

  // Call the function
  rec arg;
  arg.x = 3;
  foo(arg);

  // Shutdown the Chapel runtime and standard modules
  chpl_library_finalize();

  return 0;
}
