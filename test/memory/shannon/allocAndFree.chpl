_extern def _chpl_malloc(number, size, description, lineno=-1, filename=""): opaque;
_extern def _chpl_calloc(number, size, description, lineno=-1, filename=""): opaque;
_extern def _chpl_realloc(ptr, number, size, description, lineno=-1, filename=""): opaque;
_extern def _chpl_free(ptr, lineno=-1, filename="");

_extern def resetMemStat();
_extern def printMemStat(lineno=-1, filename="");

resetMemStat();

var i = _chpl_malloc(1, numBytes(int(64)), "int(64)", -1, "");
writeln("malloc'd an int");
printMemStat();

// numBytes(bool) == 0 which seems wrong
// but I'm not sure one should be able to query numBytes(bool anyway,
// and believe that we're going to want to make the width vary from
// implementation to implementation, so am changing the following
// from a bool to an int(8)
//
var b = _chpl_malloc(1, numBytes(int(8)), "fake bool", -1, "");
writeln("malloc'd a bool");
printMemStat();

var f = _chpl_calloc(1, numBytes(real), "real", -1, "");
writeln("calloc'd a real");
printMemStat();

_chpl_free(i);
_chpl_free(b);
writeln("freed the int and the bool");
printMemStat();

f = _chpl_realloc(f, 10, numBytes(real), "_real64", -1, "");
writeln("realloc'd 10 times the real");
printMemStat();

_chpl_free(f);
writeln("freed the real");
printMemStat();
