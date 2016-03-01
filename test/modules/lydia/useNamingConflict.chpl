module A {
  var foo = 11;
}

module B {
  var foo = 3.7;
}

module UsesBoth {
  use A, B;

  proc main() {
    writeln(foo);
  }
}
