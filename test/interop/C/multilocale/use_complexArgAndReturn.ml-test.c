#include "lib/complexArgAndReturn.h"

// Test of calling an exported Chapel library.
int main(int argc, char* argv[]) {
  // Initialize the Chapel runtime and standard modules
  chpl_library_init(argc, argv);

  // Call the function
  double complex a = 3.0 + 3.0 * I;
  take(a);
  double complex b = give();
  take(b);
  double complex d = takeAndReturn(a);
  take(d);

  // Shutdown the Chapel runtime and standard modules
  chpl_library_finalize();

  return 0;
}
