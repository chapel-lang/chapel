// Verifies that more than one module in the import statement works for both
// with unqualified access of a single symbol
module A {
  var x: int;
}
module B {
  var y: bool;
}
module User {
  import A.x, B.y;

  proc main() {
    writeln(x);
    writeln(y);
  }
}
