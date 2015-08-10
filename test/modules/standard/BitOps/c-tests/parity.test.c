#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

#include "chpl-bitops.h"

int main(int argc, char** argv) {
  int start = 0;
  int i;
  // we need to make the start dynamic so everything isn't expanded into
  // constants
  if(argc < 2) {
    return 1;
  }
  start = atoi(argv[1]);

  printf("%s", "uint32_t:\n");
  for(i=start; i<start+10; i++) {
    printf("%" PRIu32 "\n", chpl_bitops_parity_32((uint32_t)i));
  }

  printf("%s", "uint64_t:\n");
  for(i=start; i<start+10; i++) {
    printf("%" PRIu64 "\n", chpl_bitops_parity_64((uint64_t)i));
  }

  return 0;
}
