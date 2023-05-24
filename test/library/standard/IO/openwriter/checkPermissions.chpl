use IO, OS.POSIX, FileSystem;

var filename = "readOnly.txt";

var w = openWriter(filename);
w.writeln("something");
w.close();

chmod(c_ptrToConst_helper(filename):c_string, 0o0400: mode_t); // set it to read only now that it has contents

try {
  var f = openWriter(filename);
  writeln("Uh oh, didn't trigger an error");
} catch e: PermissionError {
  writeln(e.message());
} catch e {
  writeln("caught some other error");
  writeln(e.message());
}
