#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

#define LCG_MUL64 6364136223846793005ULL
#define LCG_ADD64 1

#define NUPDATE 1000

int main(int argc, char* argv[]) {
  printf("RAStream is: \n");
  uint64_t Ran = 1;

  int i;
  for (i=0; i<NUPDATE; i++) {
    Ran = LCG_MUL64 * Ran + LCG_ADD64;
    printf("%"PRIu64"\n", Ran);
  }
  
  return 0;
}
