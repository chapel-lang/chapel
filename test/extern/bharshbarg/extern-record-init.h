
#include <stdint.h>
#include <inttypes.h>

typedef struct {
  int64_t a;
  int64_t b;
  int64_t c;
  int64_t d;
} R;

static void mutateR(R* r) {
  r->a = 100;
  r->d = 42;
}

static void printR(const R* r) {
  printf("R {\n");
  printf("  a = %" PRId64 "\n", r->a);
  printf("  b = %" PRId64 "\n", r->b);
  printf("  c = %" PRId64 "\n", r->c);
  printf("  d = %" PRId64 "\n", r->d);
  printf("}\n");
}
