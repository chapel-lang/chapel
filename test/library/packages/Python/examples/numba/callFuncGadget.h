#ifndef CALL_FUNC_GADGET_H_
#define CALL_FUNC_GADGET_H_

static inline long callFunc(void* func, long x);
static inline long callFunc(void* func, long x) {
  return ((long (*)(long))func)(x);
}

#endif
