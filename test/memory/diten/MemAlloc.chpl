module MemAlloc {
  _extern proc chpl_malloc(number, size, description,
                          lineno=-1, filename=""): opaque;
  _extern proc chpl_realloc(ptr, number, size, description,
                           lineno=-1, filename=""): opaque;
  _extern proc chpl_free(ptr, lineno=-1, filename="");
}
