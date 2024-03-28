#include "lib/PtrConstQualifier.h"

int main(int argc, char** argv) {
  chpl_library_init(argc, argv);
  const int n = 8;
  const int* ptr = printAndReturn(&n);
  assert(ptr == &n);
  chpl_library_finalize();
  return 0;
}
