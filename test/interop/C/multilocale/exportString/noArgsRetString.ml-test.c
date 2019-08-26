#include "lib/TestLibrary.h"

int main(int argc, char** argv) {
  chpl_library_init(argc, argv);
  char* msg = noArgsRetString();
  printf("%s\n", msg);
  chpl_free(msg);
  chpl_library_finalize();
  return 0;
}
