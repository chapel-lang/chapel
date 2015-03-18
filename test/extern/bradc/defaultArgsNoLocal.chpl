extern proc chpl_mem_realloc(ptr, size, description, lineno=-1, filename:c_string=""): opaque;

var x: opaque;
chpl_mem_realloc(x, numBytes(int(64)), 0);
writeln("This seems to have worked");
