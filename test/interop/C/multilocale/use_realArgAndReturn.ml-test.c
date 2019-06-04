#include "lib/realArgAndReturn.h"

// Test of calling an exported Chapel library.
int main(int argc, char* argv[]) {
  // Initialize the Chapel runtime and standard modules
  chpl_library_init(argc, argv);
  chpl__init_realArgAndReturn(1, 2);

  // Call the function
  double a = 3.2;
  take(a);
  double b = give();
  take(b);
  double c = 7.8;
  double d = takeAndReturn(c);
  take(d);

  // Shutdown the Chapel runtime and standard modules
  chpl_library_finalize();

  return 0;
}
