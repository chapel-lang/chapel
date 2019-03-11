#include <stdint.h>

int baz(void);
extern int x;
extern uint32_t y;

typedef int myType;
int useMyType(myType arg);

typedef struct {
  int64_t a;
} rec;
