def assert(test: bool) {
  if (!test) {
    stdout.flush();
    stderr.write("Assertion failed");
    if chpl_input_lineno != 0 then
      stderr.writeln(": ", chpl_input_filename, ":", chpl_input_lineno);
    else
      stderr.writeln();
    exit(0);
  }
}

def assert(test: bool, args ...?numArgs) {
  if (!test) {
    stdout.flush();
    stderr.write("Assertion failed");
    if chpl_input_lineno != 0 then
      stderr.writeln(": ", chpl_input_filename, ":", chpl_input_lineno);
    else
      stderr.write(": ");
    for param i in 1..numArgs do
      stderr.write(args(i));
    stderr.writeln();
    exit(0);
  }
}

def halt() {
  stdout.flush();
  stderr.write("Halt reached");
  if chpl_input_lineno != 0 then
    stderr.writeln(": ", chpl_input_filename, ":", chpl_input_lineno);
  else
    stderr.writeln();
  exit(0);
}

def halt(args ...?numArgs) {
  stdout.flush();
  stderr.write("Halt reached");
  if chpl_input_lineno != 0 then
    stderr.writeln(": ", chpl_input_filename, ":", chpl_input_lineno);
  else
    stderr.write(": ");
  for param i in 1..numArgs do
    stderr.write(args(i));
  stderr.writeln();
  exit(0);
}
