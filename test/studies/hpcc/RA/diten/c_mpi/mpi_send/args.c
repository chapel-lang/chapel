#include <stdio.h>
#include <string.h>
#include "ra.h"
#include "args.h"

static unsigned long long my_exp2(int i) {
  unsigned long long val = 1;
  return val << i;
}

void parseArgs(int argc, char* argv[]) {
  int i;
  for (i=1; i<argc; i++) {
    if (!strncmp(argv[i], "--n=", 4)) {
      n = atoi(argv[i]+4);
    } else if (!strncmp(argv[i], "--N_U_log2=", 11)) {
      N_U_log2 = atoi(argv[i]+11);
    } else {
      printf("Unrecognized option: \"%s\"\n");
    }
  }
  m = my_exp2(n);
  idxMask = m - 1;
  N_U = my_exp2(N_U_log2);
}

