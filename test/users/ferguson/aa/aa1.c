#include <stdio.h>

void foo(long n, long *p, long *q)
{
  long i;

  if( n <= 0 ) return;

  foo(n/2, p, q);

  for(i = 1; i <= n; i++ ) {
    *p += *q;
  }

  //printf("After foo %li p=%li q=%li\n", n, *p, *q);
}

int main(int argc, char** argv)
{
  long p = 1;
  long q = 1;
  long s = 1;
  foo(10, &q, &q);
  printf("q=%li\n", q);
  foo(12, &p, &s);
  printf("p=%li s=%li\n", p, s);
  return 0;
}
