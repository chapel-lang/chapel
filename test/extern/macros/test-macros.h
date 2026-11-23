#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define ComplexFnLike() \
  printf("hi\n")

#define ComplexFnLikeArgs(arg) \
  printf("hi %i\n", arg)

#define MACRO_NEXT() next_macro_value()
#define MACRO_SUM(a, b) ((a) + (b))
#define MACRO_SUM_ALIAS MACRO_SUM
#define MACRO_TWICE(a) ((a) + (a))
#define MACRO_DEREF(p) (*(p))
#define MACRO_DISCARD(a) next_macro_value()
#define MACRO_MUTATE(a) (++(a))
#define MACRO_ADDR(a) (&(a))
#define MACRO_STRINGIFY(a) #a
#define MACRO_VARIADIC(...) (__VA_ARGS__)

#define INFER_INT_KIND(x) _Generic((x), int: 1, int64_t: 2, default: 0)
#define INFER_FLOAT_KIND(x) _Generic((x), float: 1, double: 2, default: 0)
#define INFER_PTR_KIND(p) _Generic((p), int *: 1, double *: 2, default: 0)
#define INFER_PTR_VALUE(p) (*(p))
#define INFER_PAIR_KIND(a, b) \
  (_Generic((a), int: 10, float: 20, default: 0) + \
   _Generic((b), double: 1, int *: 2, default: 0))
#define INFER_CSTRING_LEN(s) \
  (_Generic((s), char *: 1, const char *: 1, \
                  signed char *: 1, const signed char *: 1, \
                  unsigned char *: 1, const unsigned char *: 1) * \
   strlen((const char *)(s)))
#define INFER_ONLY_FLOAT(x) _Generic((x), float: 1)
#define INFER_ONLY_DOUBLE_FLOAT(x, y) \
  (_Generic((x), double: 1) + _Generic((y), float: 1))
#define ONLY_INT(x) _Generic((x), int: 1)
#define INFER_ONLY_INT_PTR(p) _Generic((p), int *: 1)

#define MAKE_UNSIGNED(a) \
  unsigned a

#define ComplexType \
  MAKE_UNSIGNED(int)

static int next_macro_value(void) {
  static int value = 0;
  return ++value;
}

#define ComplexVar next_macro_value()

#define SHIFT_CONSTANT (1U << 6)

#define ADD_CONSTANT (1+1)



#define ANSWER 42

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
