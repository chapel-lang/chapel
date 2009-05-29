use Memory;

_extern def chpl_malloc(number, size, description, lineno=-1, filename=""): opaque;
_extern def chpl_free(ptr, lineno=-1, filename="");

config var linenum: int = 0;
config var filename: string = "";

var freedInt = chpl_malloc(1, numBytes(int(64)), "int64_t", linenum, filename);
var unfreedFloat = chpl_malloc(1, numBytes(real(64)), "_real64", linenum, filename);

printMemTable(2);
chpl_free(freedInt, linenum, filename);
printMemTable(2);
