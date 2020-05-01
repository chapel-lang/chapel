module LongName {
  var x: int;
}
module L {
  var x: bool;
}
module User {
  use LongName as L;

  proc main() {
    writeln(L.x); // Should work, the L defined on line 4 was not used
    writeln(x);
  }
}
