extern proc chpl_mem_realloc(ptr, number, size, description, lineno=-1, filename=""): opaque;

var x: opaque;
chpl_mem_realloc(x, 1, numBytes(int(64)), 0);
writeln("This seems to have worked");
