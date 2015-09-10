use FileSystem;

var filename = "foo.txt";
var dirname = "../";
writeln(sameFile(filename, dirname));
// Comparing a file to a directory.  Should print false.
