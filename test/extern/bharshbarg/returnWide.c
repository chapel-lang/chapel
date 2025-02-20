
#include <stdlib.h>
#include <assert.h>
#include "returnWide.h"

#define BUF_SIZE 20

static int counter = 0;
static data* buffer[BUF_SIZE];

data* getNewData(void) {
  assert(counter < BUF_SIZE);
  static int value = 0;

  data* ret = (data*)malloc(sizeof(data));
  ret->x = value++;

  buffer[counter++] = ret;

  return ret;
}

void cleanup(void) {
  for (int i = 0; i < counter; i++) {
    free(buffer[i]);
  }
}
