// Verifies that more than one module in the import statement works for both
// with qualified access
module A {
  var x: int;
}
module B {
  var y: bool;
}
module User {
  import A, B;

  proc main() {
    writeln(A.x);
    writeln(B.y);
  }
}
