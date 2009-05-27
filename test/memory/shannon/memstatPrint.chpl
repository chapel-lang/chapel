_extern def chpl_malloc(number, size, description, lineno=-1, filename=""): opaque;
_extern def printMemStat(lineno=0, filename="");

config var print: bool = false;

printMemStat();
chpl_malloc(1, 64, "64 bytes");
printMemStat();
chpl_malloc(1, 64, "64 bytes");
printMemStat();
chpl_malloc(1, 64, "64 bytes");
printMemStat();
chpl_malloc(1, 64, "64 bytes");
printMemStat();

