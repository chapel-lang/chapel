module A {
  var x: int = 1;
}

module B {
  var x: real = 2.2;
}

module M {
  use A;
  proc main() {
    use B;
    writeln(x);
  }
}
