use FileSystem;
use IO;

var f: file = open("blah.txt", iomode.r);
var separateRec: file = open("blah.txt", iomode.r);
writeln(sameFile(f, f)); // same open file record
writeln(sameFile(f, separateRec)); // two file records, same file referred to.
writeln(sameFile("blah.txt", "blah.txt")); // same file names
writeln(sameFile("../", "../")); // same directory names.

f.close();
separateRec.close();
