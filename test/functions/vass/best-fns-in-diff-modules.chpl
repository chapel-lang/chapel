// Overloads for the same function come from different modules.

var global = 5;

module M {
  proc access() ref {
    writeln("REF");
    return global;
  }
}
module P {
  proc access() const ref {
    writeln("CONST REF");
    return global;
  }
}
module Q {
  proc access() {
    writeln("VALUE");
    return global;
  }
}
proc main {
  use M, P, Q;
  ref r = access();
  const ref c = access();
  var v = access();
}
