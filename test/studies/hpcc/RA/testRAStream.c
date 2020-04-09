#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

#define POLY 0x7

#define NUPDATE 1000

int main(int argc, char* argv[]) {
  printf("RAStream is: \n");
  uint64_t Ran = 1;

  int i;
  for (i=0; i<NUPDATE; i++) {
    Ran = (Ran << 1) ^ (((int64_t)Ran < 0) ? POLY : 0);
    printf("%"PRIu64"\n", Ran);
  }
  
  return 0;
}
