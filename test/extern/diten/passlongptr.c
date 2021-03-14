#include <stdio.h>
#include "passlongptr.h"

void f_long(long* l) {
  printf("l=%d\n", (int)*l);
}

void f_longlong(long long* l) {
  printf("ll=%d\n", (int)*l);
}
