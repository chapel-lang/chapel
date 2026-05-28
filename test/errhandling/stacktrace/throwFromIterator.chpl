iter myiter() throws {
  for i in 1..10 {
    if i == 5 then
      throw new Error("oops");
    yield i;
  }
}

proc foobar() throws {
  for i in myiter() { }
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
