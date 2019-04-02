// Make sure we link appropriately from C.
#include "lib/FooLibrary.h"

void spoutWisdom(void) {
  wisdom();
}

int main(int argc, char** argv) {
  chpl_library_init(argc, argv);
  chpl__init_FooLibrary(0, 0);
  spoutWisdom();
  chpl_library_finalize();
  return 0;
}
  
