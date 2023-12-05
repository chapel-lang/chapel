module A {
  var A: int;
}

module Main {
  use A;
  proc main() {
    writeln(A);
  }
}
