#include <stdio.h>
#include "cToChapelArray.h"

extern void chapelProc(int *arr);

void cProc(int *a)
{
  int i;
  printf("Hello from cProc\n");
  for( i = 0; i < 10; i++ ) a[i] = 10*a[i];
  for( i = 0; i < 10; i++ ) printf("a[%i] is %i\n", i, a[i]);
  printf("calling Chapel\n");
  chapelProc(a);
  printf("cProc done\n");
}
