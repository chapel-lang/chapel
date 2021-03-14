module LongName {
  var x: int;
}
module User {
  use LongName as L; // Either fail on this line

  enum L { red, blue, yellow };

  proc main() {
    writeln(L.x); // Or fail on this line, L is ambiguous
  }
}
