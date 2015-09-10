use FileSystem;
use IO;

var nameA = "oneOfTwo.txt";
var nameB = "twoOfTwo.txt";
// These files have the same contents, permissions, owners, etc.
// They are different files, though.
var fileA = open(nameA, iomode.r);
var fileB = open(nameB, iomode.r);

writeln(sameFile(fileA, fileB));
writeln(sameFile(nameA, nameB));

fileA.close();
fileB.close();
