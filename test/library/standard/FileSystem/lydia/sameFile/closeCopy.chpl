use FileSystem;
use IO;

var nameA = "oneOfTwo.txt";
var nameB = "twoOfTwo.txt";
// These files have the same contents, permissions, owners, etc.
// They are different files, though.
var fileA = open(nameA, ioMode.r);
var fileB = open(nameB, ioMode.r);

writeln(sameFile(fileA.path, fileB.path));
writeln(sameFile(nameA, nameB));

fileA.close();
fileB.close();
