#ifndef _chpl_mem_H_
#define _chpl_mem_H_

#ifndef LAUNCHER

#include <stddef.h>
#include <stdint.h>
#include "arg.h"
#include "chpltypes.h"
#include "chpltasks.h"

//
// chpl_rt_memDescs is an array of descriptions of memory allocations
// used in the runtime.  When defining new strings (for new instances
// of chpl_malloc or chpl_realloc in the runtime), be sure to define a
// new enumerated constant directly below used to index into the
// array.  Then you can use your enumeration in the call to
// chpl_malloc or chpl_realloc.  The compiler generates a separate
// array of descriptions; indices into the compiler-generated array
// start after the last index into this array.  This array is externed
// immediately after the definition of the enumeration.
//
#ifndef CHPL_MEM_C
extern const char* chpl_rt_memDescs[];
#else
const char* chpl_rt_memDescs[] = {
  "unknown",
  "chapel runtime code data",
  "chapel execution command buffer",
  "array elements",
  "set wide string",
  "get wide string",
  "private broadcast data",
  "remote fork data",
  "remote fork non-blocking data",
  "remote GPC data",
  "remote GPC header addr",
  "remote GPC header",
  "remote GPC copy of data",
  "remote GPC fork data",
  "active message fork data",
  "active message fork arg",
  "active message fork large data",
  "active message fork non-blocking data",
  "active message fork non-blocking arg",
  "active message fork non-blocking large data",
  "remote fork arg",
  "command buffer",
  "pvm list of nodes",
  "pvm spawn thing",
  "private objects array",
  "garbage collection heap",
  "garbage collection space pointer",
  "glom strings data",
  "string copy data",
  "string concat data",
  "string strided select data",
  "config arg copy data",
  "config table data",
  "locale name buffer",
  "serial flag",
  "task descriptor",
  "task descriptor link",               // reserved for future use
  "task stack",                         // reserved for future use
  "mutex",
  "lock report data",
  "task pool descriptor",
  "task list descriptor",
  "arrays for localesPerRealm",
  "thread private data",
  "thread callee function pointer and argument",
  "thread list descriptor"
};
#endif
typedef enum {
  CHPL_RT_MD_UNKNOWN = 0,
  CHPL_RT_MD_CHAPEL_CODE,
  CHPL_RT_EXECUTION_COMMAND,
  CHPL_RT_MD_ARRAY_ELEMENTS,
  CHPL_RT_MD_SET_WIDE_STRING,
  CHPL_RT_MD_GET_WIDE_STRING,
  CHPL_RT_MD_PRIVATE_BROADCAST_DATA,
  CHPL_RT_MD_REMOTE_FORK_DATA,
  CHPL_RT_MD_REMOTE_NB_FORK_DATA,
  CHPL_RT_MD_REMOTE_GPC_DATA,
  CHPL_RT_MD_REMOTE_GPC_HEADER_ADDR,
  CHPL_RT_MD_REMOTE_GPC_HEADER,
  CHPL_RT_MD_REMOTE_GPC_COPY_OF_DATA,
  CHPL_RT_MD_REMOTE_GPC_FORK_DATA,
  CHPL_RT_MD_AM_FORK_DATA,
  CHPL_RT_MD_AM_FORK_ARG,
  CHPL_RT_MD_AM_FORK_LARGE_DATA,
  CHPL_RT_MD_AM_NB_FORK_DATA,
  CHPL_RT_MD_AM_NB_FORK_ARG,
  CHPL_RT_MD_AM_NB_FORK_LARGE_DATA,
  CHPL_RT_MD_REMOTE_FORK_ARG,
  CHPL_RT_MD_COMMAND_BUFFER,
  CHPL_RT_MD_PVM_LIST_OF_NODES,
  CHPL_RT_MD_PVM_SPAWN_THING,
  CHPL_RT_MD_PRIVATE_OBJECTS_ARRAY,
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
  CHPL_RT_MD_LOCALES_PER_REALM,
  CHPL_RT_MD_THREAD_PRIVATE_DATA,
  CHPL_RT_MD_THREAD_CALLEE,
  CHPL_RT_MD_THREAD_LIST_DESCRIPTOR,
  CHPL_RT_MD_NUM
} chpl_rt_enum_memDescs;
extern const char* chpl_memDescs[];
extern const int chpl_num_memDescs;
typedef int16_t chpl_memDescInt_t;
const char* chpl_memDescString(chpl_memDescInt_t mdi);

void chpl_initHeap(void* start, size_t size);

#define CHPL_ALLOC_PERMIT_ZERO(s,d,l,f) ((s == 0) ? NULL : chpl_alloc(s,d,l,f))
#define chpl_alloc(size, description, lineno, filename) \
  chpl_malloc(1, size, description, lineno, filename)
void* chpl_malloc(size_t number, size_t size, chpl_memDescInt_t description,
                  int32_t lineno, chpl_string filename);
void* chpl_realloc(void* ptr, size_t number, size_t size, 
                   chpl_memDescInt_t description,
                   int32_t lineno, chpl_string filename);
void  chpl_free(void* ptr, int32_t lineno, chpl_string filename);

extern int heapInitialized;

void chpl_md_initHeap(void* start, size_t size);
void* chpl_md_malloc(size_t chunk, int32_t, chpl_string);
void chpl_md_free(void* memAlloc, int32_t, chpl_string);
void* chpl_md_realloc(void* memAlloc, size_t newChunk, int32_t, chpl_string);

#else // LAUNCHER

#include <stdlib.h>
#include "arg.h"

#define chpl_malloc(number, size, description, lineno, filename)        \
  malloc((number)*(size))

#define chpl_free(ptr, lineno, filename)        \
  free(ptr)

#endif // LAUNCHER

#endif
