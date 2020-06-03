#include "lib/TestLibrary.h"

int main(int argc, char** argv) {
  chpl_library_init(argc, argv);

  chpl_byte_buffer a, b;
  a = chpl_byte_buffer_make("This message is first.");
  b = chpl_byte_buffer_make("This message is second.");

  stringArgsRetVoid(a, b);

  chpl_library_finalize();
  return 0;
}
