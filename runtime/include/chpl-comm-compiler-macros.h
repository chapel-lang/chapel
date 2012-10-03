#ifndef _chpl_comm_compiler_macros_h_
#define _chpl_comm_compiler_macros_h_

#ifndef LAUNCHER

#include "chpl-mem.h"

//
// Multi-locale macros used for compiler code generation
//
// Note: Macros starting with CHPL_COMM involve some kind of communication
//


static ___always_inline
void chpl_gen_comm_get(void *addr, int32_t locale, void* raddr,
                       int32_t elemSize, int32_t typeIndex, int32_t len,
                       int ln, chpl_string fn)
{
  if (chpl_localeID == locale) {
    memcpy(addr, raddr, elemSize*len);
  } else {
#ifdef CHPL_TASK_COMM_GET
    chpl_task_comm_get(addr, locale, raddr, elemSize, typeIndex, len, ln, fn);
#else
    chpl_comm_get(addr, locale, raddr, elemSize, typeIndex, len, ln, fn);
#endif
  }
}

static ___always_inline
void chpl_gen_comm_put(void* addr, int32_t locale, void* raddr,
                       int32_t elemSize, int32_t typeIndex, int32_t len,
                       int ln, chpl_string fn)
{
  if (chpl_localeID == locale) {
    memcpy(raddr, addr, elemSize*len);
  } else {
#ifdef CHPL_TASK_COMM_PUT
    chpl_task_comm_put(addr, locale, raddr, elemSize, typeIndex, len, ln, fn);
#else
    chpl_comm_put(addr, locale, raddr, elemSize, typeIndex, len, ln, fn);
#endif
  }
}

static ___always_inline
void chpl_gen_comm_get_strd(void *addr, void *dststr, int32_t locale, void *raddr,
                       void *srcstr, void *count, int32_t strlevels, 
                       int32_t elemSize, int32_t typeIndex,
                       int ln, chpl_string fn)
{
#ifdef CHPL_TASK_COMM_GET_STRD
  chpl_task_comm_get_strd(addr, dststr, locale, raddr, srcstr, count, strlevels, elemSize, typeIndex, ln, fn);
#else
  chpl_comm_get_strd(addr, dststr, locale, raddr, srcstr, count, strlevels, elemSize, typeIndex, ln, fn);
#endif
}

static ___always_inline
void chpl_gen_comm_put_strd(void *addr, void *dststr, int32_t locale, void *raddr,
                       void *srcstr, void *count, int32_t strlevels, 
                       int32_t elemSize, int32_t typeIndex,
                       int ln, chpl_string fn)
{
#ifdef CHPL_TASK_COMM_PUT_STRD
  chpl_task_comm_put_strd(addr, dststr, locale, raddr, srcstr, count, strlevels, elemSize, typeIndex, ln, fn);
#else
  chpl_comm_put_strd(addr, dststr, locale, raddr, srcstr, count, strlevels, elemSize, typeIndex, ln, fn);
#endif
}

#include "error.h"

static ___always_inline
void chpl_test_local(int32_t locale, int32_t ln, const char* file, const char* error)
{
  if( locale != chpl_localeID ) {
    chpl_error(error, ln, file);
  }
}

#define CHPL_HEAP_REGISTER_GLOBAL_VAR(i, wide)            \
  do {                                                    \
    (wide).locale = 0;                                    \
    chpl_globals_registry[i] = (&((wide).addr));          \
    CHPL_HEAP_REGISTER_GLOBAL_VAR_EXTRA(i, wide)          \
  } while (0)

//
// If we're in serial mode, we should use blocking rather than
// non-blocking comm forks in order to serialize the forks.
// See test/parallel/serial/bradc/serialDistributedForall.chpl
// for a motivating example that didn't work before this change.
//
static ___always_inline
void chpl_comm_nonblocking_on(int32_t locale, chpl_fn_int_t fid,
                              void *arg, int32_t arg_size, int32_t arg_tid) {
  if (chpl_task_getSerial()) {
    chpl_comm_fork(locale, fid, arg, arg_size, arg_tid);
  } else {
    chpl_comm_fork_nb(locale, fid, arg, arg_size, arg_tid);
  }
}

#ifdef DEBUG_COMM_INIT
#define CHPL_COMM_DEBUG_BROADCAST_GLOBAL_VARS(numGlobals) \
  for (int i = 0; i < numGlobals; i++) \
    printf("[%d] chpl_globals_registry[%d] = %p\n", chpl_localeID, i, *((void **)chpl_globals_registry[i]));
#else
#define CHPL_COMM_DEBUG_BROADCAST_GLOBAL_VARS(numGlobals) ;
#endif

static ___always_inline
void chpl_gen_comm_broadcast_global_vars(int numGlobals)
{
  chpl_comm_barrier("barrier before broadcasting globals"); 
  chpl_comm_broadcast_global_vars(numGlobals);
  CHPL_COMM_DEBUG_BROADCAST_GLOBAL_VARS(numGlobals);
  chpl_comm_barrier("barrier after broadcasting globals");
}

static ___always_inline
void chpl_check_nil(void* ptr, int32_t lineno, const char* filename)
{
  if (ptr == nil)
    chpl_error("attempt to dereference nil", lineno, filename);
}

static ___always_inline
void* chpl_array_alloc(size_t nmemb, size_t eltSize, int32_t lineno, const char* filename) {
  return (nmemb == 0) ? (void*)(0x0) : chpl_mem_allocMany(nmemb, eltSize, CHPL_RT_MD_ARRAY_ELEMENTS, lineno, filename);
}

static ___always_inline
void* chpl_wide_array_alloc(int32_t dstLocale, size_t nmemb, size_t eltSize, int32_t lineno, const char* filename) {
  if (dstLocale != chpl_localeID)
    chpl_error("array vector data is not local", lineno, filename);;
  return chpl_array_alloc(nmemb, eltSize, lineno, filename);
}

static ___always_inline
void chpl_array_free(void* x, int32_t lineno, const char* filename)
{
  chpl_mem_free(x, lineno, filename);
}
static ___always_inline
void chpl_wide_array_free(int32_t dstLocale, void* x, int32_t lineno, const char* filename)
{
  if (dstLocale != chpl_localeID)
    chpl_error("array vector data is not local", lineno, filename);
  chpl_array_free(x, lineno, filename);
}

#endif // LAUNCHER

#endif
