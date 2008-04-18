_extern def chpl_malloc(number, size, description, lineno=-1, filename=""): opaque;
_extern def chpl_realloc(ptr, number, size, description, lineno=-1, filename=""): opaque;
_extern def printMemTable(thresh=1000, lineno=-1, filename="");


var makeMeZero = chpl_malloc(1, numBytes(int(64)), "int64_t", -1, "");
printMemTable(0);
chpl_realloc(makeMeZero, 0, numBytes(int(64)), "int64_t");
writeln("After a call to realloc with size equal to 0:");
printMemTable(0);
