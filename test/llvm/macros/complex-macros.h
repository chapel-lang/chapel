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

#define SHIFT_CONSTANT (1U << 6)

#define ADD_CONSTANT (1+1)



#define DEC_17 17

#define HEX_x17 0x0017

#define OCTAL_o10 010

#define PAREN_17 (17)

#define NEG_ONE (-1)

#define PLUS_ONE (+1)

#define PARENS_NEG_ONE (NEG_ONE)

#define CASTED_INT_10 ((int) 10)

#define RENAME myvar_impl
static int myvar_impl = 20;
