module MemAlloc {
  _extern def chpl_malloc(number, size, description,
                          userCode=true, lineno=-1, filename=""): opaque;
  _extern def chpl_realloc(ptr, number, size, description,
                           lineno=-1, filename=""): opaque;
  _extern def chpl_free(ptr, userCode=true, lineno=-1, filename="");
}
