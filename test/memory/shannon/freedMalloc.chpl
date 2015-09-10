use Memory;

extern proc chpl_mem_allocMany(number, size, description, lineno=-1, filename:c_string=""): opaque;
extern proc chpl_mem_free(ptr, lineno=-1, filename:c_string="");

config var linenum: int = 0;
config var filename = "";

var freedInt = chpl_mem_allocMany(1000, numBytes(int(64)), 0, linenum, filename.c_str());
var unfreedFloat = chpl_mem_allocMany(1000, numBytes(real(64)), 0, linenum, filename.c_str());

printMemAllocs(2000);
chpl_mem_free(freedInt, linenum, filename.c_str());
printMemAllocs(2000);
