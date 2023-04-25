use FileSystem;
use IO;

var f1: file = open("blah.txt", ioMode.r);
var f2: file = open("foo.txt", ioMode.r);
writeln(sameFile(f1.path, f2.path)); // different open file
writeln(sameFile("blah.txt", "foo.txt")); // different file names
writeln(sameFile("../", "./")); // different directory names.

f1.close();
f2.close();
