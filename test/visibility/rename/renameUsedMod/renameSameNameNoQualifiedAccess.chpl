module LongName {
  var x: int;
}
module L {
  var y: bool;
}
module User {
  use L;
  use LongName as L; // Either fail on this line

  proc main() {
    writeln(x); // Or allow this to run, since the qualified name is never used
    writeln(y);
  }
}
