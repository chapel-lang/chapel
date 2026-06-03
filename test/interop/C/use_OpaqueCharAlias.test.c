#include "lib/OpaqueCharAlias.h"

int main(int argc, char** argv) {
  chpl_library_init(argc, argv);
  foo(argv);
  chpl_library_finalize();
  return 0;
}
