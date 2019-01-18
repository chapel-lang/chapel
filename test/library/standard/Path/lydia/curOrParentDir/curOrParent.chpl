use Path;
use FileSystem;

var filename = "foo.txt";
var anotherVersion = curDir + pathSep + filename;
var parentDirectory = parentDir + pathSep + filename;

if (sameFile(filename, anotherVersion)) {
  writeln("Sweet, curDir and pathSep worked");
} else {
  writeln("Expected " + anotherVersion + " to refer to the same file as " + filename);
}

if (sameFile("../" + filename, parentDirectory)) {
  writeln("parentDir matched as expected");
} else {
  writeln("error testing parentDir, it gave us " + parentDirectory);
}
