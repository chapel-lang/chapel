#include <stdlib.h>
#include <stdio.h>
#include "streamWrapper.h"

void vector_add_cuda(float *a, float *b, float *c, float alpha, int n);

void vector_add(float *a, float *b, float *c, float alpha, int n) {
    vector_add_cuda(a, b, c, alpha, n);
}
