#include "lib/TestLibrary.h"

int main(int argc, char** argv) {
  chpl_library_init(argc, argv);
  stringArgsRetVoid("This message is first.", "This message is second.");
  chpl_library_finalize();
  return 0;
}
