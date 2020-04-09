#include <stdio.h>
#include "float4.h"

float4 getfloat4(void) {
  float4 val;
  val.x = 1.0;
  val.y = 2.0;
  val.z = 3.0;
  val.w = 4.0;
 
  return val;
}

void printme(float4 val) {
  printf("(%f, %f, %f, %f)\n", val.x, val.y, val.z, val.w);
}
