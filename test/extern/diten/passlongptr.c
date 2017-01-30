#include <stdio.h>
#include "passlongptr.h"

void f_long(long* l) {
  printf("l=%d\n", (int)*l);
}
