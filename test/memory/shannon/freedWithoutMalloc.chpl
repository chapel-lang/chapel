_extern def chpl_free(ptr, userCode=true, lineno=-1, filename="");
_extern def getRandomPtr(): opaque;

var cantFreeMe = getRandomPtr();
chpl_free(cantFreeMe, true, 5, "freedWithoutMalloc.chpl");
