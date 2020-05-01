module LongName {
  var x: int;
}
module User {
  use LongName as L; // Either fail on this line

  enum L { red, blue, yellow };

  proc main() {
    writeln(x); // Or allow this to run, since the qualified name is never used
  }
}
