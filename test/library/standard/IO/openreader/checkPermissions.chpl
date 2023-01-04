use IO, OS.POSIX, FileSystem;

var filename = "writeOnly.txt";

try {
  var f = openreader(filename);
  writeln("Uh oh, didn't trigger an error");
} catch e: PermissionError {
  writeln(e.message());
} catch e {
  writeln("caught some other error");
  writeln(e.message());
}
