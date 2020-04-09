#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <errno.h>
#include <string.h>
#include "setLimit.h"

void
setThreadLimit(int64_t limit) {
  struct rlimit rl;
  if (getrlimit(RLIMIT_NPROC, &rl) != 0) {
    fprintf(stderr, "getrlimit() failed: %s\n", strerror(errno));
  }
  rl.rlim_cur = limit;
  if (setrlimit(RLIMIT_NPROC, &rl) != 0) {
    fprintf(stderr, "setrlimit() failed: %s\n", strerror(errno));
  }
}

void
setStackLimit(int64_t limit) {
  struct rlimit rl;
  if (getrlimit(RLIMIT_STACK, &rl) != 0) {
    fprintf(stderr, "getrlimit() failed: %s\n", strerror(errno));
  }
  rl.rlim_cur = limit;
  if (setrlimit(RLIMIT_STACK, &rl) != 0) {
    fprintf(stderr, "setrlimit() failed: %s\n", strerror(errno));
  }
}

#ifdef DEBUG
int
getThreadLimit() {
  struct rlimit rl;
  if (getrlimit(RLIMIT_NPROC, &rl) != 0) {
    fprintf(stderr, "getrlimit() failed: %s\n", strerror(errno));
  }
  return(rl.rlim_cur);
}

int
getStackLimit() {
  struct rlimit rl;
  if (getrlimit(RLIMIT_STACK, &rl) != 0) {
    fprintf(stderr, "getrlimit() failed: %s\n", strerror(errno));
  }
  return(rl.rlim_cur);
}

int
main(int argc, char *argv[]) {
  int newlim = argc > 1 ? atoi(argv[1]) : 10;
  printf("%d\n", getThreadLimit());
  setThreadLimit(newlim);
  printf("%d\n", getThreadLimit());
  newlim = argc > 2 ? atoi(argv[2]) : 10;
  printf("%d\n", getStackLimit());
  setStackLimit(newlim);
  printf("%d\n", getStackLimit());
  return 0;
}

#endif
