proc foo() throws {
  coforall i in 1..10 {
    if i == 5 then
      throw new Error("oops");
  }
}
proc bar() throws {
  foo();
}
proc foobar() throws {
  bar();
}

config param doCatch = false;
proc catchIt() where doCatch {
  try! {
    foobar();
  } catch e: TaskErrors {
    writeln("caught error: ", e.message());
    writeln("stack trace:");
    for ((f, l), idx) in zip(e.stacktrace(), 1..) {
      writeln("  (frame ", idx, ") ", f, ":", l);
    }
    for t in e {
      writeln("caught error: ", t!.message());
      writeln("stack trace:");
      for ((f, l), idx) in zip(t!.stacktrace(), 1..) {
        writeln("  (frame ", idx, ") ", f, ":", l);
      }
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
