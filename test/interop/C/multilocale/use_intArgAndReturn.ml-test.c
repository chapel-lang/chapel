#include "lib/intArgAndReturn.h"

// Test of calling an exported Chapel library.
int main(int argc, char* argv[]) {
  // Initialize the Chapel runtime and standard modules
  chpl_library_init(argc, argv);

  // Call the function
  int64_t a = 3;
  take(a);
  int64_t b = give();
  take(b);
  int64_t c = 7;
  int64_t d = takeAndReturn(c);
  take(d);

  // Shutdown the Chapel runtime and standard modules
  chpl_library_finalize();

  return 0;
}
