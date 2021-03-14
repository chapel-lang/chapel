#include "lib/checkMultipleLocales.h"

int main(int argc, char* argv[]) {
  // Initialize the Chapel runtime and standard modules
  chpl_library_init(argc, argv);

  // Call the function
  foo(2);

  chpl_library_finalize();
  return 0;
}
