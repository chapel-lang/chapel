use FileSystem;
use IO;

var f: file = open("blah.txt", ioMode.r);
var separateRec: file = open("../sameFile/blah.txt", ioMode.r);
writeln(sameFile(f.path, separateRec.path)); // two file records, same file referred to.
writeln(sameFile("blah.txt", "../sameFile/blah.txt"));
// same file, one path longer than other
writeln(sameFile("../", "../../lydia/")); // same directory, one path longer.

f.close();
separateRec.close();
