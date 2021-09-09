#include "mydata.h"

#include <stdio.h>
#include <stdlib.h>

struct mydata {
  int x;
};

struct mydata* getNewData(void) {
  struct mydata* d = malloc(sizeof(struct mydata));
  d->x = 5;
  return d;
}

void acceptData(struct mydata* d) {
  printf("in acceptData d->x is %i\n", d->x);
}
