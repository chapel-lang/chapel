proc foo() throws {
  throw new Error("oops");
}
proc bar() throws {
  foo();
}
proc foobar() throws {
  bar();
}

config param doCatch = false;
proc catchIt() where doCatch {
  try {
    foobar();
  } catch e {
    writeln("caught error: ", e.message());
    writeln("stack trace:");
    for ((f, l), idx) in zip(e.stacktrace(), 1..) {
      writeln("  (frame ", idx, ") ", f, ":", l);
    }
  }
}
proc haltAndCatchFire() {
  try! foobar();
}
proc main() {
  if doCatch then catchIt();
  haltAndCatchFire();
}
