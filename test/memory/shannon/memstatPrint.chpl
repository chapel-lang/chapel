_extern def _chpl_malloc(number, size, description, lineno=-1, filename=""): opaque;
_extern def printMemStat(lineno=0, filename="");

config var print: bool = false;

printMemStat();
_chpl_malloc(1, 64, "64 bytes");
printMemStat();
_chpl_malloc(1, 64, "64 bytes");
printMemStat();
_chpl_malloc(1, 64, "64 bytes");
printMemStat();
_chpl_malloc(1, 64, "64 bytes");
printMemStat();

