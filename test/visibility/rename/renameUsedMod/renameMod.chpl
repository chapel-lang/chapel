module LongName {
  var x: int;
}
module User {
  use LongName as L;

  proc main() {
    writeln(L.x); // Basic functionality test
    writeln(x);
  }
}
