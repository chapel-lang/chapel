use Memory;

extern proc chpl_mem_allocMany(number, size, description, lineno=-1, filename:c_string=""): opaque;

config var print: bool = false;

printMemStat();
chpl_mem_allocMany(1, 64, 0);
printMemStat();
chpl_mem_allocMany(1, 64, 0);
printMemStat();
chpl_mem_allocMany(1, 64, 0);
printMemStat();
chpl_mem_allocMany(1, 64, 0);
printMemStat();

