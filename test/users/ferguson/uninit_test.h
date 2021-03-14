#include <inttypes.h>

typedef struct mys {
  int32_t a;
  int32_t b;
} mys;

static inline struct mys ret_mys(void) {
  struct mys ret;
  ret.a = 2;
  ret.b = 3;
  return ret;
}

static inline void set_mys(struct mys* x) {
  x->a = 3;
  x->b = 4;
}


