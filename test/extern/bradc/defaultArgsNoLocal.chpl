_extern def chpl_realloc(ptr, number, size, description, lineno=-1, filename=""): opaque;

var x: opaque;
chpl_realloc(x, 1, numBytes(int(64)), "int64");
writeln("This seems to have worked");
