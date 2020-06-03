use FileSystem, IO;

// Tests the case where a file record that points to null is provided as the
// only arguments to sameFile.
var nullFile: file;
writeln(sameFile(nullFile, nullFile));
