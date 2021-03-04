#include <sys/types.h>

union _someUnion {
    float x;
    double y;
};

union _noTypedefUnion {
    float x;
    double y;
    int64_t z;
};

typedef union _someUnion _someUnion;

_someUnion getSomeUnion(void);
void printSomeUnion(_someUnion _union);

union _noTypedefUnion getNoTypedefUnion(void);
void printNoTypedefUnion(union _noTypedefUnion _union);
