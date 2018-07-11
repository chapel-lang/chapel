module A {
  proc aproc() {
    writeln("public aproc");
    aproc(1);
  }

  private proc aproc(x: int) {
    writeln("private aproc ", x);
  }
}

module B {
  use A;
  proc main() {
    aproc();
  }
}
