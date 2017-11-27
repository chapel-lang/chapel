use Path;
use FileSystem;

var unmodified = "onFileInDir.chpl";
var f = open(unmodified, iomode.r);
// I know I exist (update name if changed)
var result = f.realPath();
var expected = here.cwd() + pathSep + unmodified;
// Will want to use joinPath here when it is implemented.
if (result != expected) {
  writeln("Expected " + expected + " but got " + result);
} else {
  writeln("realPath on a path without symlinks, curDir, or parentDir references behaved as expected");
 }
