use IO, OS.POSIX, FileSystem;

var filename = "readOnly.txt";

var w = openwriter(filename);
w.writeln("something");
w.close();

chmod(filename.c_str(), 0o0400: mode_t); // set it to read only now that it has contents

try {
  var f = open(filename, ioMode.cw);
  writeln("Uh oh, didn't trigger an error");
} catch e: PermissionError {
  writeln(e.message());
} catch e {
  writeln("caught some other error");
  writeln(e.message());
}
