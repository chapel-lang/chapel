use IO;
use Path;

var f = open("f.txt", ioMode.r);
writeln(f.path);

// ensuring that this doesn't throw a deprecation warning from module code
writeln(absPath(f));
