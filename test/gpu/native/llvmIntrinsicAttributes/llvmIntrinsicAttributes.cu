#include <stdio.h>
#include <stdlib.h>

__global__ void sample_kernel() {
    int i = threadIdx.x;
    printf("thread %d\n", i);
}

int main() {
  sample_kernel<<<1,1>>>();
}
