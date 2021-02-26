module A {
  var x: int;
}
module User {
  use A only x as _; // This line should fail

  proc main() {
    writeln(A.x); // If the above line works, this should be fine, though
  }
}
