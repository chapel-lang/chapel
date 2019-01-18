#define _ISOC11_SOURCE 1
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <myth/myth.h>

#if ! HAVE_DECL_ALIGNED_ALLOC
void * aligned_alloc(size_t al, size_t sz);
#endif

int main(int argc, char ** argv) {
  size_t al = (argc > 1 ? atol(argv[1]) : 32);
  size_t sz = (argc > 2 ? atol(argv[2]) : 35);
  size_t n  = (argc > 3 ? atol(argv[3]) : 3);
  size_t i;
  for (i = 0; i < n; i++) {
    char * a = (char *)aligned_alloc(al, sz);
    if (!a) { perror("aligned_alloc"); exit(1); }
    if (((long)a) % al) {
      fprintf(stderr,
	      "aligned_alloc(%ld,%ld) returned a pointer %p not aligned as specified\n",
	      al, sz, a);
      exit(1);
    }
    size_t j;
    for (j = 0; j < sz; j++) a[j] = 1;
  }
  printf("OK\n");
  return 0;
}

