_extern def chpl_free(ptr, lineno=-1, filename="");
_extern def getRandomPtr(): opaque;

var cantFreeMe = getRandomPtr();
chpl_free(cantFreeMe, 5, "freedWithoutMalloc.chpl");
