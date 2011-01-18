#include <stdint.h>

typedef struct myclass_s {
  int64_t a;
} _myclass, *myclass;

int64_t myfunc(myclass c);
