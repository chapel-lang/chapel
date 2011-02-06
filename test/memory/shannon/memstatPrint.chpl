use Memory;

_extern proc chpl_malloc(number, size, description, lineno=-1, filename=""): opaque;

config var print: bool = false;

printMemStat();
chpl_malloc(1, 64, 0);
printMemStat();
chpl_malloc(1, 64, 0);
printMemStat();
chpl_malloc(1, 64, 0);
printMemStat();
chpl_malloc(1, 64, 0);
printMemStat();

