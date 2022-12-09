use IO, OS.POSIX;

var filename = "secretlyNotADir/foo.txt";

try {
  var f = open(filename, iomode.cw);
  writeln("uh oh, no error");
} catch e: NotADirectoryError {
  writeln(e.message());

} catch e {
  writeln("uh oh, different error");
  writeln(e.message());
}
