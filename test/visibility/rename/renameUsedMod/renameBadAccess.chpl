module LongName {
  var x: int;
}
module User {
  use LongName as L;

  proc main() {
    writeln(LongName.x);
    writeln(x);
  }
}
