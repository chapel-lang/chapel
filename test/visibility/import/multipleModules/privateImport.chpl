module A {
  var x: int;
}
module B {
  var y = true;
  var z = "blah blah";
}
module C {
  private import A.x, B;
}
module User {
  use C;

  proc main() {
    writeln(x); // Verifies that the import of A was actually private
  }
}
