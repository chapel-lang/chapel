module LongName {
  var x: int;
}
module User {
  use LongName as L;

  proc main() {
    writeln(LongName.x); // Should fail, renamed LongName when using it
    writeln(x);
  }
}
