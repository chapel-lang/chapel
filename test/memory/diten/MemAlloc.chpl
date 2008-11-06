module MemAlloc {
  _extern def chpl_malloc(number, size, description,
                          lineno=-1, filename=""): opaque;
  _extern def chpl_realloc(ptr, number, size, description,
                           lineno=-1, filename=""): opaque;
  _extern def chpl_calloc(number, size, description,
                          lineno=-1, filename=""): opaque;
  _extern def chpl_free(ptr, lineno=-1, filename="");
}
