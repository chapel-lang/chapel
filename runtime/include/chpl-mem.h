#ifndef _chpl_mem_H_
#define _chpl_mem_H_

#ifndef LAUNCHER

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "arg.h"
#include "chpltypes.h"
#include "chpl-tasks.h"
#include "error.h"

// runtime/include/mem/*/chpl-mem-impl.h defines
// chpl_calloc, chpl_malloc, chpl_realloc, chpl_free
// with the same signatures as the standard functions
// and no additional error checking.
#include "chpl-mem-impl.h"

// import allocation types
#include "chpl-mem-desc.h"

// Need memory tracking prototypes for inlined memory routines
#include "chplmemtrack.h"

// CHPL_MEM_DEBUG=1 will enable tracking and other extra checks;
// CHPL_MEM_DEBUG will be set to 1 if CHPL_DEBUG is defined;
// or if CHPL_OPTIMIZE is not defined.
// If CHPL_OPTIMIZE is defined and CHPL_DEBUG is not defined,
// we set CHPL_MEM_DEBUG to chpl_memTrack, so that memory tracking
// can still be activated at run-time.
#ifndef CHPL_MEM_DEBUG

#ifdef CHPL_DEBUG
#define CHPL_MEM_DEBUG 1
#else
#ifdef CHPL_OPTIMIZE
#define CHPL_MEM_DEBUG chpl_memTrack
#else
#define CHPL_MEM_DEBUG 1
#endif
#endif

#endif

extern const int chpl_mem_numDescs; // defined in generated code

const char* chpl_mem_descString(chpl_mem_descInt_t mdi);

void chpl_mem_init(void);
void chpl_mem_exit(void);

//
// Inform callers as to the memory layer's actual starting address
// and length for the shared heap, if that is known.  The *start_p
// value will be set to NULL if the memory layer does not know this
// information.
//
void chpl_mem_actualSharedHeap(void** start_p, size_t* size_p);

#define chpl_mem_allocPermitZero(s,d,l,f) ((s == 0) \
                                           ? NULL \
                                           : chpl_mem_alloc(s,d,l,f))
#define chpl_mem_alloc(size, description, lineno, filename) \
  chpl_mem_allocMany(1, size, description, lineno, filename)

void chpl_mem_check_pre(size_t number, size_t size, chpl_bool zeroOK,
                         chpl_mem_descInt_t description,
                         int32_t lineno, chpl_string filename);
void chpl_mem_check_post(void* memAlloc,
                         chpl_mem_descInt_t description,
                         int32_t lineno, chpl_string filename);

static ___always_inline
void* chpl_mem_allocMany(size_t number, size_t size,
                         chpl_mem_descInt_t description,
                         int32_t lineno, chpl_string filename) {
  void* memAlloc;
  if( CHPL_MEM_DEBUG ) {
    chpl_mem_check_pre(number, size, false, description, lineno, filename);
  }
  memAlloc = chpl_malloc(number*size);
  if( CHPL_MEM_DEBUG || ! memAlloc ) {
    chpl_mem_check_post(memAlloc, description, lineno, filename);
  }
  if( CHPL_MEM_DEBUG ) {
    chpl_track_malloc(memAlloc, number*size, number, size, description,
                      lineno, filename);
  }
  return memAlloc;
}

static ___always_inline
void chpl_mem_free(void* memAlloc, int32_t lineno, chpl_string filename) {
  if( CHPL_MEM_DEBUG ) {
    // call this one just to check heap is initialized.
    chpl_mem_check_pre(0, 0, true, 0, lineno, filename);
    chpl_track_free(memAlloc, lineno, filename);
  }
  chpl_free(memAlloc);
}

static ___always_inline
void* chpl_mem_realloc(void* memAlloc, size_t number, size_t size, 
                       chpl_mem_descInt_t description,
                       int32_t lineno, chpl_string filename) {
  void* moreMemAlloc;
  if( number == 0 || size == 0 ) {
    chpl_mem_free(memAlloc, lineno, filename);
    return NULL;
  }
  if( CHPL_MEM_DEBUG ) {
    chpl_mem_check_pre(number, size, true, description, lineno, filename);
    chpl_track_realloc1(memAlloc, number, size, description, lineno, filename);
  }
  moreMemAlloc = chpl_realloc(memAlloc, number*size);
  if( CHPL_MEM_DEBUG || ! moreMemAlloc ) {
    chpl_mem_check_post(moreMemAlloc, description, lineno, filename);
  }
  if( CHPL_MEM_DEBUG ) {
    chpl_track_realloc2(moreMemAlloc, number*size, memAlloc, number, size,
                        description, lineno, filename);
  }
  return moreMemAlloc;
}

extern int heapInitialized;

void chpl_mem_layerInit(void);
void chpl_mem_layerExit(void);
void chpl_mem_layerActualSharedHeap(void**, size_t*);
void* chpl_mem_layerAlloc(size_t, int32_t, chpl_string);
void* chpl_mem_layerRealloc(void*, size_t, int32_t, chpl_string);
void chpl_mem_layerFree(void*, int32_t, chpl_string);

#else // LAUNCHER

#include <stdlib.h>
#include "arg.h"

#define chpl_mem_allocMany(number, size, description, lineno, filename)        \
  malloc((number)*(size))

#define chpl_mem_free(ptr, lineno, filename)        \
  free(ptr)

#endif // LAUNCHER

#endif
