#ifndef _chpl_mem_H_
#define _chpl_mem_H_

#ifndef LAUNCHER

#include <stddef.h>
#include <stdint.h>
#include "arg.h"
#include "chpltypes.h"
#include "chpl-tasks.h"

//
// When defining a new allocation type (for new instances of chpl_mem_allocMany
// or chpl_mem_realloc in the runtime), define both a new enumeration constant
// below and the corresponding descriptive string in the table defined in
// chpl-mem.c:chpl_mem_descString().  Then you can use your enumeration in
// a call to chpl_mem_allocMany(), chpl_mem_realloc(), or chpl_mem_descString().
//
typedef enum {
  CHPL_RT_MD_UNKNOWN = 0,
  CHPL_RT_MD_CHAPEL_CODE,
  CHPL_RT_EXECUTION_COMMAND,
  CHPL_RT_MD_ARRAY_ELEMENTS,
  CHPL_RT_MD_SET_WIDE_STRING,
  CHPL_RT_MD_GET_WIDE_STRING,
  CHPL_RT_MD_COMMAND_BUFFER,
  CHPL_RT_MD_COMM_XMIT_RECV_BUF,
  CHPL_RT_MD_COMM_XMIT_RECV_STATUS,
  CHPL_RT_MD_COMM_FORK_SEND_INFO,
  CHPL_RT_MD_COMM_FORK_SEND_NB_INFO,
  CHPL_RT_MD_COMM_FORK_SEND_LARGE_ARG,
  CHPL_RT_MD_COMM_FORK_SEND_NB_LARGE_ARG,
  CHPL_RT_MD_COMM_FORK_SEND_RESPONSE_DATA,
  CHPL_RT_MD_COMM_FORK_RECV_INFO,
  CHPL_RT_MD_COMM_FORK_RECV_LARGE_INFO,
  CHPL_RT_MD_COMM_FORK_RECV_NB_INFO,
  CHPL_RT_MD_COMM_FORK_RECV_NB_LARGE_INFO,
  CHPL_RT_MD_COMM_FORK_RECV_LARGE_ARG,
  CHPL_RT_MD_COMM_FORK_RECV_NB_LARGE_ARG,
  CHPL_RT_MD_COMM_FORK_DONE_FLAG,
  CHPL_RT_MD_COMM_PER_LOCALE_INFO,
  CHPL_RT_MD_COMM_PRIVATE_OBJECTS_ARRAY,
  CHPL_RT_MD_COMM_PRIVATE_BROADCAST_DATA,
  CHPL_RT_MD_GC_HEAP,
  CHPL_RT_MD_GC_SPACE_POINTER,
  CHPL_RT_MD_GLOM_STRINGS_DATA,
  CHPL_RT_MD_STRING_COPY_DATA,
  CHPL_RT_MD_STRING_CONCAT_DATA,
  CHPL_RT_MD_STRING_STRIDED_SELECT_DATA,
  CHPL_RT_MD_CONFIG_ARG_COPY_DATA,
  CHPL_RT_MD_CONFIG_TABLE_DATA,
  CHPL_RT_MD_LOCALE_NAME_BUFFER,
  CHPL_RT_MD_SERIAL_FLAG,
  CHPL_RT_MD_TASK_DESCRIPTOR,
  CHPL_RT_MD_TASK_DESCRIPTOR_LINK,      // reserved for future use
  CHPL_RT_MD_TASK_STACK,                // reserved for future use
  CHPL_RT_MD_MUTEX,
  CHPL_RT_MD_LOCK_REPORT_DATA,
  CHPL_RT_MD_TASK_POOL_DESCRIPTOR,
  CHPL_RT_MD_TASK_LIST_DESCRIPTOR,
  CHPL_RT_MD_THREAD_PRIVATE_DATA,
  CHPL_RT_MD_THREAD_LIST_DESCRIPTOR,
  CHPL_RT_MD_NUM
} chpl_mem_rtMemDesc_t;
extern const int chpl_mem_numDescs;
typedef int16_t chpl_mem_descInt_t;
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
void* chpl_mem_allocMany(size_t number, size_t size,
                         chpl_mem_descInt_t description,
                         int32_t lineno, chpl_string filename);
void* chpl_mem_realloc(void* ptr, size_t number, size_t size, 
                       chpl_mem_descInt_t description,
                       int32_t lineno, chpl_string filename);
void  chpl_mem_free(void* ptr, int32_t lineno, chpl_string filename);

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
