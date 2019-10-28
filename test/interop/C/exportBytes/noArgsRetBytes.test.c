#include "lib/TestLibrary.h"

//
// This test verifies some of the behavior of the `chpl_bytes` wrapper that
// is used to communicate the bytes type between Chapel and Python. It is
// not intended to be used directly in C interop code yet.
//
int main(int argc, char** argv) {
  chpl_library_init(argc, argv);
  
  chpl_bytes_wrapper msg = noArgsRetBytes();

  printf("cb.isOwned: %s\n", (msg.isOwned ? "true" : "false"));
  printf("cb.data: %s\n", msg.data);
  printf("cb.size: %zu\n", msg.size);

  chpl_bytes_wrapper_free(msg);

  chpl_library_finalize();

  return 0;
}
