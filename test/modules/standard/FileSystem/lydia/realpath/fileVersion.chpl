use FileSystem;
use IO;

var filename = "../realpath/./blahblahblah.txt";
var openedFile = open(filename, iomode.cw);

var path = openedFile.realPath();
var shouldMatch = here.cwd() + "/blahblahblah.txt";
// May want to us joinPath and basename here when available.
if path != shouldMatch {
  writeln("Whoops, expected " + shouldMatch + " but got " + path);
} else {
  writeln("They matched!");
}
