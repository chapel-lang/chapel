// This is from #27764.
module ContextSensitiveThrows {
  record R {
    var x: int;
    proc enterContext() ref do return x;
    proc exitContext(in e: owned Error?) throws {
      if e then throw e;
    }
  }

  R implements contextManager;

  proc main() {
    var r: R;
    manage r as x do x += 1;
    writeln(r);
  }
}
