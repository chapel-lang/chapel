#include <iostream>

#include <errno.h>
#include <stdint.h>
#include <sys/types.h>

#include "tcmalloc-interface.h"

#include "base/basictypes.h"
#include "base/googleinit.h"
#include "system-alloc.h"


//
// This is only referred to by the tcmalloc code in third-party/, and that
// declares it itself instead of #including our tcmalloc-interface.h file,
// so there's no need to declare it publicly.
//
SysAllocator *chpl_mem_createStaticPoolSysAllocator(void);


#ifdef HEAP_MUST_BE_CONTIGUOUS
//
// Our heap needs to be contiguous, probably because the comm layer requires
// it.  We'll need a staticly allocated memory pool to operate on until we
// can acquire the large memory pool the program will use.
//

//
// hugetlbfs.h doesn't do the extern "C" thing itself, so we have to.
//
extern "C" {
#include <hugetlbfs.h>
}

//
// The StaticPoolSysAllocator is a static allocator for the tcmalloc
// system allocator module.  It is only used when we are building with
// the uGNI-based communication layer.  In that case most of the
// memory is supplied by the hugepage-based system allocator below,
// but that cannot be created early enough to satisy all needs.  We
// must avoid use of tcmalloc's built-in sbrk- and mmap-based system
// allocators because we cannot support dynamic expansion of the heap,
// which has to be registered with the NIC via uGNI.  So we use this
// little static system allocator to supply memory to tcmalloc until
// our main hugepage allocator can be set up.
//
class StaticPoolSysAllocator : public SysAllocator {
public:
  StaticPoolSysAllocator() : SysAllocator() {
  }
  void* Alloc(size_t size, size_t *actual_size, size_t alignment);
  void DumpStats(TCMalloc_Printer* printer);
private:
  size_t curr_offset_;
};

static const size_t max_size_ = 4 * 1024 * 1024;
static char heap_base_[max_size_];
static char staticpool_space[sizeof(StaticPoolSysAllocator)];

void* StaticPoolSysAllocator::Alloc(size_t size, size_t* actual_size,
                                    size_t alignment)
{
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

void StaticPoolSysAllocator::DumpStats(TCMalloc_Printer* printer) { }


SysAllocator *chpl_mem_createStaticPoolSysAllocator(void)
{
  return new (staticpool_space) StaticPoolSysAllocator();
}


class ChapelHugepageSysAllocator: public SysAllocator {
public:
  ChapelHugepageSysAllocator(size_t size, void* base)
    : max_size_(size),
      heap_base_(base),
      curr_offset_(0) {
  }

  void* Alloc(size_t size, size_t* actual_size, size_t alignment);

  void DumpStats(TCMalloc_Printer* printer);

private:
  size_t max_size_;
  void* heap_base_;
  size_t curr_offset_;
};


void ChapelHugepageSysAllocator::DumpStats(TCMalloc_Printer* printer) {
  printer->printf("ChapelHugeSysAllocator: base=%"PRIu64", size=%"PRId64"\n",
                  reinterpret_cast<uint64_t>(heap_base_), max_size_);
}


void* ChapelHugepageSysAllocator::Alloc(size_t size, size_t* actual_size,
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


void* ChapelHugepageSysAllocatorInit(size_t size, size_t* actual_size) {
  //
  // get_huge_pages() requires a size aligned to the hugepage size.  Align
  // the requested size as necessary.
  //
  size_t hugepage_size = gethugepagesize();
  size_t s = ((size + hugepage_size - 1) / hugepage_size) * hugepage_size;
  void*  p = get_huge_pages(s, GHP_DEFAULT);

  if (p == NULL)
    return NULL;

  SysAllocator* a = new ChapelHugepageSysAllocator(s, p);
  RegisterSystemAllocator(a, 0);

  if (actual_size != NULL)
    *actual_size = s;
  return p;
}


extern "C"
void* tcmallocChapelInit_c(size_t size, size_t* actual_size) {
  return ChapelHugepageSysAllocatorInit(size, actual_size);
}


#else // ifdef HEAP_MUST_BE_CONTIGUOUS
//
// Our heap need not be contiguous.  We don't need the staticly allocated
// memory pool, and we can let tcmalloc acquire system memory as needed.
//

SysAllocator *chpl_mem_createStaticPoolSysAllocator(void)
{
  return NULL;
}


extern "C"
void* tcmallocChapelInit_c(size_t size, size_t* actual_size) {
  return NULL;
}

#endif // ifdef HEAP_MUST_BE_CONTIGUOUS
