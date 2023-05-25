use FileSystem;
use Path;
use IO;

var filename = "../realpath/./blahblahblah.txt";
var openedFile = open(filename, ioMode.cw);

var path = realPath(openedFile);
var shouldMatch = here.cwd() + "/blahblahblah.txt";
// May want to use joinPath and basename here when available.
if path != shouldMatch {
  writeln("Whoops, expected " + shouldMatch + " but got " + path);
} else {
  writeln("They matched!");
}
