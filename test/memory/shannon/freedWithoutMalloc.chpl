_extern def _chpl_free(ptr, lineno=-1, filename="");
_extern def getRandomPtr(): opaque;

var cantFreeMe = getRandomPtr();
_chpl_free(cantFreeMe, 5, "freedWithoutMalloc.chpl");
