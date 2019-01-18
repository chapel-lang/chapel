#include <stdio.h>

#include "myassert.h"

void assert_match(int64_t got, int64_t expected, int32_t lineno, int32_t filenum)
{
  if( got != expected ) {
    printf("assert failed on line %i. got=%li, expected=%li\n",
           lineno, (long int) got, (long int) expected);
  }
}


