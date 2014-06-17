#include <stdint.h>

typedef struct myclass_s {
  int64_t a;
  int64_t b;
  int64_t c;
  int64_t d;
  int64_t e;
} _myclass, *myclass;

int64_t myfunc(myclass c);
