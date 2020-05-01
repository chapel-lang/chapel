use FileSystem, IO;

// Tests the case where a file record that points to null is provided as the
// first argument to sameFile.
var hasStuff: file = open("blah.txt", iomode.r);
var nullFile: file;
writeln(sameFile(nullFile, hasStuff));
hasStuff.close();
