#include <stdio.h>
#include "unions.h"

_someUnion getSomeUnion(void) {
    _someUnion val;
    val.x = 0.5f;
    val.y = (double) 0.42f;

    return val;
}

void printSomeUnion(_someUnion _union) {
    printf("(%f, %lf)\n", _union.x, _union.y);
}

union _noTypedefUnion getNoTypedefUnion(void) {
    union _noTypedefUnion val;
    val.x = 0.8f;
    val.y = (double) 0.43f;

    return val;
}

void printNoTypedefUnion(union _noTypedefUnion _union) {
    printf("(%f, %lf, %ld)\n", _union.x, _union.y, _union.z);
}
