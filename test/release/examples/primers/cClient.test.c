// Include the generated header file
#include "lib/interopWithC.h"

// C code to call exported functions from Chapel libraries.
int main(int argc, char* argv[]) {
  // Initialize the Chapel runtime and internal libraries
  chpl_library_init(argc, argv);
  // Initialize the compiled Chapel library and any used modules
  chpl__init_interopWithC(0, 0);

  foo();
  alsoCallsBaz();
  callUseMyType();

  // Shutdown the Chapel runtime and standard modules
  chpl_library_finalize();

  return 0;
}
