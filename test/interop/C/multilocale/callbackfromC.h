extern void foo(int64_t* x);

static inline void bar(int64_t* x) {
  foo(x);
}
