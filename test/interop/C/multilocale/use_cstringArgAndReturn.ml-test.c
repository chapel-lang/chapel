#include "lib/cstringArgAndReturn.h"

// Test of calling an exported Chapel library.
int main(int argc, char* argv[]) {
  // Initialize the Chapel runtime and standard modules
  chpl_library_init(argc, argv);

  // Call the function
  const char* a = "The string that was given";
  take(a);
  const char* b = give();
  take(b);
  const char* c = "The last of the strings";
  const char* d = takeAndReturn(c);
  take(d);
  chpl_free((char*)b);
  chpl_free((char*)d);
  // Shutdown the Chapel runtime and standard modules
  chpl_library_finalize();

  return 0;
}
