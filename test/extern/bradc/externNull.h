typedef struct _s {
  double x;
} s;

typedef s* sPtr;

s myS;

static inline sPtr getNullSPtr(void) {
  return NULL;
}

static inline sPtr getNonNullSPtr(void) {
  return &myS;
}
