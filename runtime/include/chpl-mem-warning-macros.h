#ifndef _chpl_mem_warning_macros_h_
#define _chpl_mem_warning_macros_h_

// this function gets its own ifdef in case
// we want to disable and then re-enable the warning macros.
#ifndef _chpl_mem_warning_macros_really_h_
#define _chpl_mem_warning_macros_really_h_
// MPF - I needed to call the system free to pair with system valloc
// (these are for I/O buffers that don't belong on the Chapel sharable
// heap anyway because they could be mmap'd - and then are only
// sharable when everything is sharable)
static ___always_inline void sys_free(void *ptr) { free(ptr); }
#endif

#define malloc  dont_use_malloc_use_chpl_mem_allocMany_instead
#define calloc  dont_use_calloc_use_chpl_mem_allocMany_instead
#define free    dont_use_free_use_chpl_mem_free_instead
#define realloc dont_use_realloc_use_chpl_mem_realloc_instead

#endif
