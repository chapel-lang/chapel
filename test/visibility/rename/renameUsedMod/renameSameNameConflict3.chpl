module LongName {
  var x: int;
}
module L {
  var y: bool;
}
module User {
  use LongName as L; // Either fail on this line
  use L;

  proc main() {
    writeln(L.x); // Or fail on this line, L is ambiguous
    writeln(x);
  }
}
