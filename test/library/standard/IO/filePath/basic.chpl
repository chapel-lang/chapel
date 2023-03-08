use IO;

var f_here = open("./test.txt", ioMode.r);
var f_nested = open("./nested/test.txt", ioMode.r);

writeln(f_here.path);
writeln(f_nested.path);
