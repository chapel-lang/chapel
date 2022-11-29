use IO, OS.POSIX, FileSystem;

var filename = "readOnly.txt";

var w = openwriter(filename);
w.writeln("something");
w.close();

chmod(filename, 0o0400); // set it to read only now that it has contents

try {
  var f = openwriter(filename);
  writeln("Uh oh, didn't trigger an error");
} catch e: PermissionError {
  writeln(e.message());
} catch e {
  writeln("caught some other error");
  writeln(e.message());
}
