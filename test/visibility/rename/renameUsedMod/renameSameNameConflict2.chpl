module L {
  var y: bool;
}
module LongName {
  var x: int;
}
module User {
  use L;
  use LongName as L; // Either fail on this line

  proc main() {
    writeln(L.x); // Or fail on this line, L is ambiguous
    writeln(x);
  }
}
