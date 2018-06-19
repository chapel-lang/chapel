module A {
  private proc aproc(x: int) {
    writeln("private aproc ", x);
  }

  proc aproc() {
    writeln("public aproc");
    aproc(1);
  }
}

module B {
  use A;
  proc main() {
    A.aproc();
  }
}
