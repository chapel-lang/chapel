use IO;
use OS.POSIX;

var filename = "doesNotExist.txt";
try {
  var f = open(filename, ioMode.r);
  writeln("Uh oh, didn't trigger error");

} catch e: FileNotFoundError {
  writeln(e.message());

} catch e {
  writeln("caught some other error");
  writeln(e.message());
}
