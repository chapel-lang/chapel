_extern def _chpl_realloc(ptr, number, size, description, lineno=-1, filename=""): opaque;
_extern def getRandomPtr(): opaque;

var cantReallocMe = getRandomPtr();
_chpl_realloc(cantReallocMe, 3, numBytes(int(64)), "int64_t", 5, "reallocWithoutMalloc.chpl");

