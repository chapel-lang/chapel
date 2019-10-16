
proc f() {
  writeln("in f");
  return 1;
}

proc main() {
  try {
    f();
  } catch e: Error {
    writeln(e.message());
  }
  writeln("done");
}
