module A {
  proc x() {
    writeln("In A.x()");
  }
  proc y() {
    writeln("In A.y()");
  }
}
module User {
  import A.{x as foo};

  proc main() {
    foo();
  }
}
