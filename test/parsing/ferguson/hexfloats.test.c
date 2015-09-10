#include <stdio.h>
#include <assert.h>

int main(int argc, char** argv)
{
  double x = 0x1p-1;
  assert( x == 0.5 );
  printf("x is %lf\n", x);
  return 0;
}

