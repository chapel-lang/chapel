#include "lib/TestLibrary.h"

int main(int argc, char** argv) {
  chpl_library_init(argc, argv);
  const char* msg = stringArgsRetString("Greetings", ", computer!");
  printf("%s\n", msg);
  chpl_free((void*) msg);
  chpl_library_finalize();
  return 0;
}
