module MemAlloc {
  extern proc chpl_mem_allocMany(number, size, description,
                                 lineno=-1, filename=0): opaque;
  extern proc chpl_mem_realloc(ptr, size, description,
                               lineno=-1, filename=0): opaque;
  extern proc chpl_mem_free(ptr, lineno=-1, filename=0);
}
