#include <stdint.h>

int baz(void);
extern int x;
extern uint32_t y;

typedef int myType;
int useMyType(myType arg);
int sum(int, int);
int sumArray(int64_t[], int);

typedef struct {
  int64_t a;
} rec;

typedef struct {
  int someData;
} notReallyEmpty;

struct person{
  char* name;
  int age;
};
