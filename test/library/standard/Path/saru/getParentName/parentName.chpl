use IO;
use Path;

/* Tests IO.file.getParentName() function */
proc main() {
  var f = open('testFile.txt', iomode.r);

  writeln(f.getParentName());
}
