#include <stdio.h>

#define ComplexFnLike() \
  printf("hi\n")

#define ComplexFnLikeArgs(arg) \
  printf("hi %i\n", arg)

#define MAKE_UNSIGNED(a) \
  unsigned a

#define ComplexType \
  MAKE_UNSIGNED(int)

#define ComplexVar \
  getchar()
