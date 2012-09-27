#if 0
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "chplcgfns.h"
#include "chplcopygc.h"
#include "chplrt.h"
#include "error.h"
#include "chpl-mem.h"

_memory_space *_from_space, *_to_space;

int totalRoots = 0;
void *rootlist[MAXROOTS];

void _chpl_gc_copy_collect(void) {
  int i;
  _memory_space* tmp;
  char* scanptr;
  //fprintf(stderr, "Collection\n");
  for (i=0; i < totalRoots; i++) {
    if (STACK_PTR(rootlist[i])) {
      if (HEAP_AS_PTR(rootlist[i]) >= (void*)_to_space->head &&
          HEAP_AS_PTR(rootlist[i]) < (void*)_to_space->tail) {
        STACK_PTR(rootlist[i]) = HEAP_AS_PTR(rootlist[i]);
      } else {
        if (STACK_PTR(rootlist[i]) >= (void*)_from_space->head &&
            STACK_PTR(rootlist[i]) < (void*)_from_space->tail) {
          size_t size = cid2size(*(chpl__class_id*)STACK_PTR(rootlist[i]));
          memmove(_to_space->current, STACK_PTR(rootlist[i]), size);
          HEAP_AS_PTR(rootlist[i]) = (void*)_to_space->current;
          STACK_PTR(rootlist[i]) = (void*)_to_space->current;
          _to_space->current += size;
        } else {
          /* BAD - Something in the root set points at something that
             wasn't moved, but isn't in the from-space */
        }
      }
    }
  }

  /* Now that any root objects are moved, move sub-objects. */
  scanptr = _to_space->head;
  while (scanptr != _to_space->current) {
    size_t *offsets = cid2offsets(*(chpl__class_id*)scanptr);
    if (offsets[1] == (size_t)-1) {
      // data class (array)
      int i;
      // Bad, could be uint(64) index size
      int array_size = *(int*)(((char*)scanptr) + offsets[2]);
      void** data = *(void***)(((char*)scanptr) + offsets[3]);
      for (i=0; i < array_size; i++) {
        if (data[i]) {
          if (*(void**)data[i] >= (void*)_to_space->head &&
              *(void**)data[i] < (void*)_to_space->tail) {
            /* Update the pointer to point into the new heap */
            data[i] = *(void**)data[i];
          } else {
            if (data[i] >= (void*)_from_space->head &&
                data[i] < (void*)_from_space->tail) {
              size_t size = cid2size(*(chpl__class_id*)data[i]);
              memmove(_to_space->current, data[i], size);
              /* Put an update pointer on the heap */
              *(void**)data[i] = (void*)_to_space->current;
              /* Update the pointer in the array */
              data[i] = (void*)_to_space->current;
              _to_space->current += size;
            } else {
              /* Bad */
            }
          }
        }
      }
    } else {
      int i = 1;
      while (offsets[i] != 0) {
        void* current = (void*)(scanptr + offsets[i]);
        if (STACK_PTR(current)) {
          if (HEAP_AS_PTR(current) >= (void*)_to_space->head &&
              HEAP_AS_PTR(current) < (void*)_to_space->tail) {
            STACK_PTR(current) = HEAP_AS_PTR(current);
          } else {
            if (STACK_PTR(current) >= (void*)_from_space->head &&
                STACK_PTR(current) < (void*)_from_space->tail) {
              size_t size = cid2size(**(chpl__class_id**)current);
              memmove(_to_space->current, STACK_PTR(current), size);
              HEAP_AS_PTR(current) = (void*)_to_space->current;
              STACK_PTR(current) = (void*)_to_space->current;
              _to_space->current += size;
            } else {
              /* BAD - Something pointing at a non-forwarded object that isn't
                 isn't in the from-space or NULL */
            }
          }
        }
        i++;
      }
    }
    scanptr += offsets[0];
  }

  // Swap the _from_space and the _to_space
  _from_space->current = _from_space->head;
  tmp = _from_space;
  _from_space = _to_space;
  _to_space = tmp;
}

void* _chpl_gc_malloc(size_t number, size_t size, const char* description,
                      int32_t lineno, chpl_string filename) {
  char* current = NULL;
  size_t chunk = number * size;
  if (_from_space->current + chunk > _from_space->tail) {
    // Garbage collect, then allocate
    _chpl_gc_copy_collect();
    if (_from_space->current + chunk > _from_space->tail) {
      // out of memory.  Should probably realloc the heap.
      // For now, throw an error
      char message[1024];
      sprintf(message, "Out of memory allocating \"%s\"", description);
      chpl_error(message, lineno, filename);
    } else {
      current = _from_space->current;
      _from_space->current += chunk;
    }
  } else {
    current = _from_space->current;
    _from_space->current += chunk;
  }
  return (void*)current;
}

void _chpl_gc_init(size_t heapsize) {
  char *heap1, *heap2;

  // allocate the from and to spaces
  heap1 = (char*)chpl_mem_allocMany(1, heapsize, CHPL_RT_MD_GC_HEAP, 1, "");
  heap2 = (char*)chpl_mem_allocMany(1, heapsize, CHPL_RT_MD_GC_HEAP, 1, "");

  // allocate structs to point into the spaces
  _from_space = (_memory_space*)chpl_mem_allocMany(1, sizeof(_memory_space),
                                                   CHPL_RT_MD_GC_SPACE_POINTER,
                                                   1, "");
  _to_space = (_memory_space*)chpl_mem_allocMany(1, sizeof(_memory_space),
                                                 CHPL_RT_MD_GC_SPACE_POINTER,
                                                 1, "");

  // fill in the pointers
  _from_space->head = heap1;
  _from_space->tail = heap1+heapsize;
  _from_space->current = heap1;
  _to_space->head = heap2;
  _to_space->tail = heap2 + heapsize;
  _to_space->current = heap2;
}

void _chpl_gc_cleanup(void) {
  chpl_mem_free(_to_space->head, __LINE__, __FILE__);
  chpl_mem_free(_from_space->head, __LINE__, __FILE__);
  chpl_mem_free(_to_space, __LINE__, __FILE__);
  chpl_mem_free(_from_space, __LINE__, __FILE__);
}
#endif
