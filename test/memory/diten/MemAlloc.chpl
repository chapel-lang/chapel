module MemAlloc {
  _extern proc chpl_mem_allocMany(number, size, description,
                                  lineno=-1, filename=""): opaque;
  _extern proc chpl_mem_realloc(ptr, number, size, description,
                                lineno=-1, filename=""): opaque;
  _extern proc chpl_mem_free(ptr, lineno=-1, filename="");
}
