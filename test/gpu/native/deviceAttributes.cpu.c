#include "deviceAttributes.h"
#include <stdio.h>

void runBaselineVersion(void) {
  printf("name: chapel-cpu-as-device-gpu\n");
  printf("internal error: querying gpu attributes is currently unsupported in cpu-as-device mode.\n");
}
