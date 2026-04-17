// This is from #27764.
//
// I believe I named the module this because of the desire for the 'manage'
// statement to be lowered in a context-sensitive way depending on whether
// or not:
//
//    -- The contents of the 'manage' throws
//    -- The 'exitContext' call is marked 'throws'
//    -- The 'manage' exists in a throwing scope
//
module ContextSensitiveThrows {
  record R {
    var x: int;
    proc ref enterContext() ref do return x;
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
