/* utils.cc */

#include <cstdlib>
#include <cstdio>

void * xmalloc(size_t size)
{
  void *p = malloc(size);
  if (p == NULL) {
    perror("memory allocation failed");
    exit(1);
  }
  return p;
}

double xrandom(double xl, double xh)
{
  return (xl + (xh - xl) * ((double) random()) / 2147483647.0);
}
