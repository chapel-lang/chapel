#include "lib/boolArgAndReturn.h"
#include "stdbool.h"

// Test of calling an exported Chapel library.
int main(int argc, char* argv[]) {
  // Initialize the Chapel runtime and standard modules
  chpl_library_init(argc, argv);

  // Call the function
  bool a = true;
  take(a);
  bool b = give();
  take(b);
  bool c = true;
  bool d = takeAndReturn(c);
  take(d);

  // Shutdown the Chapel runtime and standard modules
  chpl_library_finalize();

  return 0;
}
