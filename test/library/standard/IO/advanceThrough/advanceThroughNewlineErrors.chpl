use IO;

var r = openReader("advanceThroughNewlineErrors.txt");

try {
  r.advanceThroughNewline();
} catch e {
  writeln(e);
}

try {
  // try reading a newline at EOF
  r.advanceThroughNewline();
} catch e {
  writeln(e);
}
