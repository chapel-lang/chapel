//
// Error checking functions for memory debugging support.
//
#include "chplrt.h"

#include "chpl-mem.h"
#include "chpl-mem-hook.h"
#include "chpltypes.h"
#include "error.h"

#include <stdio.h>
#include <stdint.h>


void chpl_memhook_check_pre(size_t number, size_t size,
                            chpl_mem_descInt_t description,
                            int32_t lineno, chpl_string filename) {
  if (!chpl_mem_inited())
    chpl_error("memory routine called before the memory layer is initialized",
               lineno, filename);

  if (number > 0 && size > SIZE_MAX/number)
    chpl_error("Attempting to allocate > max(size_t) bytes of memory",
               lineno, filename);
}


void chpl_memhook_check_post(void* memAlloc,
                             chpl_mem_descInt_t description,
                             int32_t lineno, chpl_string filename) {
  if (memAlloc == NULL) {
    const int messageSize = 1024;
    char message[messageSize];

    snprintf(message, messageSize, "Out of memory allocating \"%s\"",
             chpl_mem_descString(description));
    chpl_error(message, lineno, filename);
  }
}
