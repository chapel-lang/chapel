#include "lib/multipleArgs.h"

// Test of calling an exported Chapel library.
int main(int argc, char* argv[]) {
  // Initialize the Chapel runtime and standard modules
  chpl_library_init(argc, argv);

  // Call the function
  int64_t a = 3;
  int64_t b = 4;
  take2(a, b);
  const char* strarg = "blah blah";
  int64_t c = 2;
  take2Diff(c, strarg);
  int64_t d = 5;
  int64_t e = take2AndReturn(c, d);
  take2(c + d, e);

  // Shutdown the Chapel runtime and standard modules
  chpl_library_finalize();

  return 0;
}
