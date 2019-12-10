#include "lib/TestLibrary.h"

int main(int argc, char** argv) {
  chpl_library_init(argc, argv);
  chpl_bytes_wrapper msg = stringArgsRetString("Greetings", ", computer!");
  printf("%s\n", msg.data);
  chpl_bytes_wrapper_Free(msg);
  chpl_library_finalize();
  return 0;
}
