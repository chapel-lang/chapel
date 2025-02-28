#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "argparsing.h"
#include <assert.h>
#include <qthread/qthread.h>
#include <stdio.h>
#include <stdlib.h>

static unsigned int target = 8;
static aligned_t x = 0;

static aligned_t alldone;

#ifdef __clang__
#define STACKLEFT_NOINLINE __attribute__((optnone))
#elif __GNUC__
#define STACKLEFT_NOINLINE __attribute__((optimize(0)))
#else
#define STACKLEFT_NOINLINE
#endif

// Macro for excluding a function from thread sanitizer.
// Currently this is just used for qt_swapctxt.
// Something about thread sanitizer's instrumentation is
// incompatible with our context switching.
#if defined(__has_feature)
#if __has_feature(thread_sanitizer)
#define QT_SKIP_THREAD_SANITIZER                                               \
  __attribute__((disable_sanitizer_instrumentation))
#else
#define QT_SKIP_THREAD_SANITIZER
#endif
#else
#define QT_SKIP_THREAD_SANITIZER
#endif

static STACKLEFT_NOINLINE size_t thread2(size_t left, size_t depth) {
  size_t foo = qthread_stackleft();
  iprintf("leveli%i: %zu bytes left\n", (int)depth, foo);
  assert(foo < left);
  if (depth < 5) { thread2(foo, depth + 1); }
  return 1;
}

static QT_SKIP_THREAD_SANITIZER aligned_t thread(void *arg) {
  int me = qthread_id();
  size_t foo = qthread_stackleft();
  iprintf("%zu bytes left\n", foo);
  thread2(foo, 2);
  assert(qthread_lock(&x) == 0);
  x++;
  if (x == target) { qthread_unlock(&alldone); }
  assert(qthread_unlock(&x) == 0);
  return foo + me; /* to force them to be used */
}

int main(int argc, char *argv[]) {
  long int i;

  assert(qthread_initialize() == 0);

  NUMARG(target, "TEST_TARGET");
  CHECK_VERBOSE();

  qthread_lock(&alldone);

  for (i = 0; i < target; i++) {
    int res = qthread_fork(thread, NULL, NULL);
    if (res != 0) { printf("res = %i\n", res); }
    assert(res == 0);
  }

  qthread_lock(&alldone);

  iprintf("Final value of x=%lu (expecting %lu)\n",
          (unsigned long)x,
          (unsigned long)target);

  qthread_unlock(&alldone);

  if (x == target) {
    return 0;
  } else {
    return -1;
  }
}

/* vim:set expandtab */
