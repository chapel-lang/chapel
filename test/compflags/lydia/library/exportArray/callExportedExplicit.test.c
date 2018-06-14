#include <stdio.h>

#include "exportFuncWithExplicitVoidRet.h"

extern void chpl_library_init(int argc, char* argv[]);
extern void chpl_library_finalize(void);

// Test of calling an exported function that takes an array
int main(int argc, char* argv[]) {
  // Initialize the Chapel runtime and standard modules
  chpl_library_init(argc, argv);
  chpl__init_exportFuncWithExplicitVoidRet(0, 0);

  // Call the function
  int64_t x[5] = {1, 2, 3, 4, 5};
  foo(x, 5);
  for (int i = 0; i < 5; i++) {
    printf("Element[%d] = %lld\n", i, x[i]);
  }

  // Shutdown the Chapel runtime and standard modules
  chpl_library_finalize();

  return 0;
}
