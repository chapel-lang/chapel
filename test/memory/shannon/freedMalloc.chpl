_extern def _chpl_malloc(number, size, description, lineno=-1, filename=""): opaque;
_extern def _chpl_free(ptr, lineno=-1, filename="");
_extern def printMemTable(thresh=1000, lineno=-1, filename="");

config var linenum: int = 0;
config var filename: string = "";

var freedInt = _chpl_malloc(1, numBytes(int(64)), "int64_t", linenum, filename);
var unfreedFloat = _chpl_malloc(1, numBytes(real(64)), "_real64", linenum, filename);

printMemTable(2, linenum, filename);
_chpl_free(freedInt, linenum, filename);
writeln("And this is the memory hash table after the int variable has been freed:");
printMemTable(2, linenum, filename);
