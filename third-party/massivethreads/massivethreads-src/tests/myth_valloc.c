#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <myth/myth.h>

int main(int argc, char ** argv) {
  size_t sz = (argc > 1 ? atol(argv[1]) : 35);
  size_t n  = (argc > 2 ? atol(argv[2]) : 3);
  size_t i;
  for (i = 0; i < n; i++) {
    char * a = (char *)valloc(sz);
    if (!a) { perror("valloc"); exit(1); }
    if (((long)a) % sysconf(_SC_PAGESIZE)) {
      fprintf(stderr,
	      "valloc(%ld) returned a pointer %p not aligned as specified\n",
	      sz, a);
      exit(1);
    }
    size_t j;
    for (j = 0; j < sz; j++) a[j] = 1;
  }
  printf("OK\n");
  return 0;
}

