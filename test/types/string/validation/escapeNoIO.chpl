use IO;

var b = b"\xff";
var s = b.decode(policy=decodePolicy.escape);

try {
  stdout.writeln(s);
  writeln("writeln shouldn't have been successful");
}
catch e: SystemError {
  writeln("Correct error was caught");
}
catch {
  writeln("Unexpected error");
}

try {
  stdout.writef("%s\n", s);
  writeln("writef('%s') shouldn't have been successful");
}
catch e: SystemError {
  writeln("Correct error was caught");
}
catch {
  writeln("Unexpected error");
}

try {
  stdout.writef("%|*s\n", s.numBytes, s);
  writeln("writef('%|s') should be successful");
}
catch {
  writeln("Unexpected error");
}

