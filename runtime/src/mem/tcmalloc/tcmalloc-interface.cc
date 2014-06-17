#include <iostream>

#include <errno.h>
#include <stdint.h>
#include <sys/types.h>

#include "tcmalloc-interface.h"

#include "base/basictypes.h"
#include "base/googleinit.h"
#include "system-alloc.h"


class ChapelSysAllocator: public SysAllocator {
public:
  ChapelSysAllocator(void* base, size_t size)
    : heap_base_(base),
      max_size_(size),
      curr_offset_(0) {
  }

  void* Alloc(size_t size, size_t* actual_size, size_t alignment);

  void DumpStats(TCMalloc_Printer* printer);

private:
  void* heap_base_;
  size_t max_size_;
  size_t curr_offset_;
};


void ChapelSysAllocator::DumpStats(TCMalloc_Printer* printer) {
  printer->printf("ChapelHugeSysAllocator: base=%"PRIu64", size=%"PRId64"\n",
                  reinterpret_cast<uint64_t>(heap_base_), max_size_);
}


void* ChapelSysAllocator::Alloc(size_t size, size_t* actual_size,
                                size_t alignment) {
  //
  // There is no locking needed here since we assume that tcmalloc calls
  // us with an internal lock held (see tcmalloc/system-alloc.cc).
  //

  //
  // Return the next chunk of the memory, if we have it to give.
  //
  uintptr_t p = ((reinterpret_cast<uintptr_t>(heap_base_)
                  + curr_offset_ + alignment - 1) / alignment) * alignment;

  size_t p_offset = p - reinterpret_cast<uintptr_t>(heap_base_);

  size = ((size + alignment - 1) / alignment) * alignment;
  if (size > max_size_ - p_offset)
    return NULL;

  if (actual_size != NULL)
    *actual_size = size;

  curr_offset_ = p_offset + size;

  return reinterpret_cast<void*>(p);
}


void ChapelSysAllocatorInit(void*, size_t);
void ChapelSysAllocatorInit(void* addr, size_t size) {
  if (addr != NULL) {
    RegisterSystemAllocator(new ChapelSysAllocator(addr, size), 0);
    DisableAllButThisSystemAllocator(0);
  }
}


extern "C"
void tcmallocChapelInit_c(void* addr, size_t size) {
  ChapelSysAllocatorInit(addr, size);
}
