use IO;

var f_here = open("./test.txt", iomode.rw);
var f_nested = open("./nested/test.txt", iomode.rw);

writeln(f_here.path);
writeln(f_nested.path);
