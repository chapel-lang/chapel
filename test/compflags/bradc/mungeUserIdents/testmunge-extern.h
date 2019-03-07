static inline void foo(void) { printf("In foo!\n"); }

typedef struct _bar {
  double foo;
} bar;

typedef struct _globStruct {
  double foo;
} globStruct;

bar baz = {4.2};
