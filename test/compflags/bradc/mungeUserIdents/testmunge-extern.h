static inline void foo(void) { printf("In foo!\n"); }

typedef struct _bar {
  double foo;
} bar;

bar baz = {4.2};
