use Memory;

extern proc chpl_mem_allocMany(number, size, description, lineno=-1, filename:c_string=""): opaque;
extern proc chpl_mem_realloc(ptr, size, description, lineno=-1, filename:c_string=""): opaque;

var makeMeZero = chpl_mem_allocMany(1, numBytes(int(64)), 0);
printMemAllocs(0);
chpl_mem_realloc(makeMeZero, 0, 0);
printMemAllocs(0);
