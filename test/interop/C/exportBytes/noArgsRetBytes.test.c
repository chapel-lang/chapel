#include "lib/TestLibrary.h"

int main(int argc, char** argv) {
  chpl_library_init(argc, argv);
  
  chpl_bytes msg = noArgsRetBytes();

  printf("cb.isOwned: %s\n", (msg.isOwned ? "true" : "false"));
  printf("cb.data: %s\n", msg.data);
  printf("cb.len: %zu\n", msg.len);

  // This should be exported but not mangled.
  chpl_bytes_free(msg);

  chpl_library_finalize();

  return 0;
}
