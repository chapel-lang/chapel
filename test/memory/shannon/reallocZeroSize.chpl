use Memory;

_extern proc chpl_malloc(number, size, description, lineno=-1, filename=""): opaque;
_extern proc chpl_realloc(ptr, number, size, description, lineno=-1, filename=""): opaque;

var makeMeZero = chpl_malloc(1, numBytes(int(64)), 0, -1, "");
printMemTable(0);
chpl_realloc(makeMeZero, 0, numBytes(int(64)), 0);
printMemTable(0);
