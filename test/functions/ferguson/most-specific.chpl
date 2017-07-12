module A {
  proc f(x:int, y) {
    writeln("A.f");
  }
}

module B {
  use A;
  proc f(x, y:int) {
    writeln("B.f");
  }

  proc main() {
    f(1,2);
  }
}
