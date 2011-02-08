use Memory;

_extern proc chpl_malloc(number, size, description, lineno=-1, filename=""): opaque;
_extern proc chpl_free(ptr, lineno=-1, filename="");

config var linenum: int = 0;
config var filename: string = "";

var freedInt = chpl_malloc(1000, numBytes(int(64)), 0, linenum, filename);
var unfreedFloat = chpl_malloc(1000, numBytes(real(64)), 0, linenum, filename);

printMemTable(2000);
chpl_free(freedInt, linenum, filename);
printMemTable(2000);
