#include <stdio.h>

typedef struct _R {
  int x;
} R;

static void foo(R r) {
  printf("foo got: %d\n", r.x);
}

static void bar(R* r) {
  printf("bar got: %d\n", r->x);
}
