void test(void);

static inline int g(void) {
  int I = 1;
  I++;
  return I;
}

static inline int f(const int* I) {
  return *I + g();
}

