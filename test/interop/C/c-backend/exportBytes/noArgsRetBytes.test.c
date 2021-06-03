#include "lib/TestLibrary.h"
#include <stdint.h>

//
// This test verifies some of the behavior of the `chpl_bytes` wrapper that
// is used to communicate the bytes type between Chapel and Python. It is
// not intended to be used directly in C interop code yet.
//
int main(int argc, char** argv) {
  chpl_library_init(argc, argv);
  
  chpl_byte_buffer msg = noArgsRetBytes();

  printf("cb.isOwned: %s\n", (msg.isOwned ? "true" : "false"));
  printf("cb.data: %s\n", msg.data);
  printf("cb.size: %" PRIu64 "\n", msg.size);

  chpl_byte_buffer_free(msg);

  chpl_library_finalize();

  return 0;
}
