/* 
 * myth_wrap_malloc.c : malloc and friends
 */

#include "myth_real.h"
#include "myth_wrap_util_func.h"

void * __wrap(malloc)(size_t size) {
  int _ = enter_wrapped_func("%lu", size);
  void * x = real_malloc(size);
  (void)_;
  leave_wrapped_func("%p", x);
  return x;
}

void __wrap(free)(void * ptr) {
  int _ = enter_wrapped_func("%p", ptr);
  (void)_;
  real_free(ptr);
  leave_wrapped_func(0);
}

void * __wrap(calloc)(size_t nmemb, size_t size) {
  int _ = enter_wrapped_func("%lu, %lu", nmemb, size);
  void * x = real_calloc(nmemb, size);
  (void)_;
  leave_wrapped_func("%p", x);
  return x;
}

void * __wrap(realloc)(void * ptr, size_t size) {
  int _ = enter_wrapped_func("%p, %lu", ptr, size);
  void * x = real_realloc(ptr, size);
  (void)_;
  leave_wrapped_func("%p", x);
  return x;
}

int __wrap(posix_memalign)(void ** memptr, size_t alignment, size_t size) {
  int _ = enter_wrapped_func("%p, %lu, %lu", memptr, alignment, size);
  int x = real_posix_memalign(memptr, alignment, size);
  (void)_;
  leave_wrapped_func("%p", x);
  return x;
}

#if HAVE_ALIGNED_ALLOC
void * __wrap(aligned_alloc)(size_t alignment, size_t size) {
  int _ = enter_wrapped_func("%lu, %lu", alignment, size);
  void * x = real_aligned_alloc(alignment, size);
  (void)_;
  leave_wrapped_func("%p", x);
  return x;
}
#endif

void * __wrap(valloc)(size_t size) {
  int _ = enter_wrapped_func("%lu", size);
  void * x = real_valloc(size);
  (void)_;
  leave_wrapped_func("%p", x);
  return x;
}

void * __wrap(memalign)(size_t alignment, size_t size) {
  int _ = enter_wrapped_func("%lu, %lu", alignment, size);
  void * x = real_memalign(alignment, size);
  (void)_;
  leave_wrapped_func("%p", x);
  return x;
}

void * __wrap(pvalloc)(size_t size) {
  int _ = enter_wrapped_func("%lu", size);
  void * x = real_pvalloc(size);
  (void)_;
  leave_wrapped_func("%p", x);
  return x;
}
