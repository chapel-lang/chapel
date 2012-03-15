use Memory;

extern proc chpl_mem_allocMany(number, size, description, lineno=-1, filename=""): opaque;
extern proc chpl_mem_realloc(ptr, number, size, description, lineno=-1, filename=""): opaque;

var makeMeZero = chpl_mem_allocMany(1, numBytes(int(64)), 0, -1, "");
printMemTable(0);
chpl_mem_realloc(makeMeZero, 0, numBytes(int(64)), 0);
printMemTable(0);
