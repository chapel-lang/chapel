use FileSystem;
use IO;

var f: file = open("blah.txt", ioMode.r);
var separateRec: file = open("blah.txt", ioMode.r);
writeln(sameFile(f.path, f.path)); // same open file record
writeln(sameFile(f.path, separateRec.path)); // two file records, same file referred to.
writeln(sameFile("blah.txt", "blah.txt")); // same file names
writeln(sameFile("../", "../")); // same directory names.

f.close();
separateRec.close();
