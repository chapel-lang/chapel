use IO;

var f_here = open("./test.txt", iomode.r);
var f_nested = open("./nested/test.txt", iomode.r);

writeln(f_here.path);
writeln(f_nested.path);
