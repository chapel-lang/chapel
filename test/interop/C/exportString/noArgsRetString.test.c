#include "lib/TestLibrary.h"

int main(int argc, char** argv) {
  chpl_library_init(argc, argv);
  chpl_byte_buffer msg = noArgsRetString();
  printf("%s\n", msg.data);
  chpl_byte_buffer_free(msg);
  chpl_library_finalize();
  return 0;
}
