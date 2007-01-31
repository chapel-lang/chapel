def assert(test: bool) {
  if (!test) {
    stdout.flush();
    stderr.write("Assertion failed");
    stderr.writeln(": ", _get_filename(), ":", _get_lineno());
    exit(0);
  }
}

def assert(test: bool, args ...?numArgs) {
  if (!test) {
    stdout.flush();
    stderr.write("Assertion failed");
    stderr.writeln(": ", _get_filename(), ":", _get_lineno());
    for param i in 1..numArgs do
      stderr.write(args(i));
    stderr.writeln();
    exit(0);
  }
}

def halt() {
  stdout.flush();
  stderr.write("Halt reached");
  stderr.writeln(": ", _get_filename(), ":", _get_lineno());
  exit(0);
}

def halt(args ...?numArgs) {
  stdout.flush();
  stderr.write("Halt reached");
  stderr.writeln(": ", _get_filename(), ":", _get_lineno());
  for param i in 1..numArgs do
    stderr.write(args(i));
  stderr.writeln();
  exit(0);
}
