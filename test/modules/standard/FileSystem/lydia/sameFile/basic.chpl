use FileSystem;
use IO;

var f: file = open("blah.txt", iomode.r);
writeln(sameFile(f, f));
writeln(sameFile("blah.txt", "foo.txt"));
