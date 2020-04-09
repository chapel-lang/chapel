#include <stdint.h>
#include <stdio.h>

static
void printem(uint8_t* x, int32_t num)
{
  int32_t i;
  for(i = 0; i < num; i++ ) {
    printf("%i ", x[i]);
  }
  printf("\n");
}

