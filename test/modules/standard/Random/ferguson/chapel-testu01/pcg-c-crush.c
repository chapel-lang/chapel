#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

#include "pcg_basic.h"


#include "unif01.h"
#include "bbattery.h"

unsigned int test_rand_uint32(void)
{
  return pcg32_random();
}

void run_smallcrush_testu01_uint(const char* name)
{
  unif01_Gen *gen;
  gen = unif01_CreateExternGenBits((char*)name, test_rand_uint32);
  bbattery_SmallCrush(gen);
  unif01_DeleteExternGenBits(gen);
}

void run_crush_testu01_uint(const char* name)
{
  unif01_Gen *gen;
  gen = unif01_CreateExternGenBits((char*)name, test_rand_uint32);
  bbattery_Crush(gen);
  unif01_DeleteExternGenBits(gen);
}



int main(int argc, char** argv)
{

  unsigned long seeds[] = {1, 589494289, 11, 31415, 2415065787, 111111,
               589494289+1000, 520326001786743721, 54553810781164123,
               845796262492190295, 944310181042360313,
               7, 17, 314159265, 271828182845, 0xfffffff};
 
  int seedi = 1;
  unsigned long seed;
  long inc = 1;
  int crush=0;
  int smallcrush=1;
  int i;

  for(i = 1; i < argc; i++ ) {
    if (0 == strcmp("--seedi", argv[i])) {
      i++;
      seedi = atoi(argv[i]);
    } else if (0 == memcmp("--seedi=", argv[i], strlen("--seedi="))) {
      seedi = atoi(&argv[i][strlen("--seedi=")]);
    } else if(0 == strcmp("--smallcrush", argv[i]) ||
              0 == strcmp("--smallcrush=true", argv[i])) {
      smallcrush=1;
      crush=0;
    } else if(0 == strcmp("--crush", argv[i]) ||
              0 == strcmp("--crush=true", argv[i]) ) {
      smallcrush=0;
      crush=1;
    }
  }

  seed = seeds[seedi-1];

  printf("seed is %lu inc is %lu\n", seed, inc);

  pcg32_srandom(seed, inc);

  if (smallcrush) run_smallcrush_testu01_uint("PCG-C-32");
  if (crush) run_crush_testu01_uint("PCG-C-32");

  return 0;
}

