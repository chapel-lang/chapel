proc foo() throws {
  throw new Error("oops");
}
proc bar() throws {
  foo();
}
proc foobar() throws {
  bar();
}

proc catchIt() {
  try {
    foobar();
  } catch e {
    writeln("caught error: ", e.message());
    for ((f, l), idx) in zip(e.stacktrace(), 1..) {
      writeln("  (frame ", idx, ") ", f, ":", l);
    }
  }
}
proc main() {
  catchIt();
}
