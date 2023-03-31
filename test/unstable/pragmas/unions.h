#include <sys/types.h>

union c_someUnion {
    float x;
    double y;
};

union c_noTypedefUnion {
    float x;
    double y;
    int64_t z;
};

typedef union c_someUnion c_someUnion;

c_someUnion getSomeUnion(void);
void printSomeUnion(c_someUnion c_union);

union c_noTypedefUnion getNoTypedefUnion(void);
void printNoTypedefUnion(union c_noTypedefUnion c_union);

typedef struct containingAnonUnnamedUnion {
  // Anonymous unnamed union
  union {
    float x;
    double y;
    int64_t z;
  };
} containingAnonUnnamedUnion;
