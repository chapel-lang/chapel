use FileSystem;
use IO;

var f1: file = open("blah.txt", iomode.r);
var f2: file = open("foo.txt", iomode.r);
writeln(sameFile(f1, f2)); // different open file
writeln(sameFile("blah.txt", "foo.txt")); // different file names
writeln(sameFile("../", "./")); // different directory names.

f1.close();
f2.close();
