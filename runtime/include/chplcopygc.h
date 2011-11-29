#ifndef _chplcopygc_H_
#define _chplcopygc_H_

#include <stddef.h>
#include <stdint.h>
#include "chpltypes.h"
#define MAXROOTS 1024

#define HEAP_AS_PTR(root) \
  (**(void***)(root))

#define STACK_PTR(root) \
  (*(void**)(root))

typedef struct {
  char* head;
  char* current;
  char* tail;
} _memory_space;

extern int totalRoots;
extern void *rootlist[MAXROOTS];

/* Initialize two heaps of size heapsize bytes */
void _chpl_gc_init(size_t heapsize);

/* Free the memory used by the collector including the heaps */
void _chpl_gc_cleanup(void);

/* Add/remove "roots".  Every varaible on the stack that can
   point at heap should be added when it is in scope, deleted
   when it leaves scope. Unless GC is currently running, these
   should all be pointers to NULL variables or pointers
   to pointers to heap */
#define _addRoot(root) rootlist[totalRoots++] = (root)
#define _addNullRoot(root) *(void**)(root) = NULL; rootlist[totalRoots++] = root
#define _deleteRoot(count) totalRoots -= (count)

/* Allocate space in the from-space.  If not enough space exists,
   run a garbage collection cycle. */
void* _chpl_gc_malloc(size_t number, size_t size, const char* description,
                      int32_t lineno, chpl_string filename);

/* Run a garbage collection cycle */
void _chpl_gc_copy_collect(void);

#endif
