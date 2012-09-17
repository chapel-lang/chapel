#ifndef _chplrt_H_
#define _chplrt_H_

#include "chpltypes.h"

#ifndef LAUNCHER

#include <stdint.h>
#include "chpl-comm-printf-macros.h"

extern int32_t chpl_localeID;

// need chpl_mem_alloc and chpl_mem_allocPermitZero
#include "chpl-mem.h"

// need chpl_error
#include "error.h"

extern chpl_fn_p chpl_ftable[];

extern int chpl_verbose_comm;     // set via startVerboseComm
extern int chpl_comm_diagnostics; // set via startCommDiagnostics
extern int chpl_verbose_mem;      // set via startVerboseMem

extern const char* chpl_compileCommand;
extern const char* chpl_compileVersion;
extern const char* CHPL_HOST_PLATFORM;
extern const char* CHPL_TARGET_PLATFORM;
extern const char* CHPL_HOST_COMPILER;
extern const char* CHPL_TARGET_COMPILER;
extern const char* CHPL_THREADS;
extern const char* CHPL_COMM;
extern char* chpl_executionCommand;

extern int chpl_threads_initialized;

// MPF - believe no longer used
#define CHPL_ASSIGN_SVEC(x, y)                     \
  memcpy(&x, &y, sizeof(x))

static ___always_inline
void chpl_check_nil(void* ptr, int32_t lineno, const char* filename)
{
  if (ptr == nil)
    chpl_error("attempt to dereference nil", lineno, filename);
}

// MPF - believe no longer used
#define _ARRAY_GET(x, i) (&((x)[i]))
#define _ARRAY_GET_VALUE(x, i) ((x)[i])
#define _ARRAY_SET(x, i, v) ((x)[i] = v)
#define _ARRAY_SET_SVEC(x, i, v) CHPL_ASSIGN_SVEC(((x)[i]), v)

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

// MPF - believe no longer used
#define _ARRAY_FREE_ELTS(x, i, call) \
  for(i = 0; i < (x)->size; i++) call

// MPF - believe no longer used
#define CHPL_VMT_CALL(vmt, cid, ind)            \
  (vmt[cid][ind])

#define _noop(x)

#include "chpl-mem-warning-macros.h"

#define exit    dont_use_exit_use_chpl_exit_instead

#endif // LAUNCHER

#endif
