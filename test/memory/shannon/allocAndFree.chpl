extern proc chpl_mem_allocMany(number, size, description, userCode=true, lineno=-1, filename:c_string=""): opaque;
extern proc chpl_mem_realloc(ptr, size, description, lineno=-1, filename:c_string=""): opaque;
extern proc chpl_mem_free(ptr, userCode=true, lineno=-1, filename:c_string="");

extern proc resetMemStat();
extern proc printMemAllocStats(lineno=-1, filename="");

resetMemStat();

var i = chpl_mem_allocMany(1, numBytes(int(64)), "int(64)", true, -1, "");
writeln("malloc'd an int");
printMemAllocStats();

// numBytes(bool) == 0 which seems wrong
// but I'm not sure one should be able to query numBytes(bool anyway,
// and believe that we're going to want to make the width vary from
// implementation to implementation, so am changing the following
// from a bool to an int(8)
//
var b = chpl_mem_allocMany(1, numBytes(int(8)), "fake bool", true, -1, "");
writeln("malloc'd a bool");
printMemAllocStats();

var f = chpl_mem_allocMany(1, numBytes(real), "real", true, -1, "");
writeln("malloc'd a real");
printMemAllocStats();

chpl_mem_free(i);
chpl_mem_free(b);
writeln("freed the int and the bool");
printMemAllocStats();

f = chpl_mem_realloc(f, 10 * numBytes(real), "_real64", -1, "");
writeln("realloc'd 10 times the real");
printMemAllocStats();

chpl_mem_free(f);
writeln("freed the real");
printMemAllocStats();
