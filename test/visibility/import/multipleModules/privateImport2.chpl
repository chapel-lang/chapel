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
    writeln(B.y); // Verifies that the import of B was also actually private
  }
}
