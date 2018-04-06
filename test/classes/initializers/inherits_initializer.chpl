// Verifies the behavior when a child that wants a default initializer inherits
// from a parent that defines an initializer, when --force-initializers not
// thrown.  When default initializers are always generated, we should delete
// this test in favor of the copy in compilerGenerated/
class Parent {
  var a: int;

  proc init() {
    a = 11;

  }
}

class Child : Parent {
  var b: int;
}

proc main() {
  var c = new Child(4);

  writeln(c);
  delete c;
}
