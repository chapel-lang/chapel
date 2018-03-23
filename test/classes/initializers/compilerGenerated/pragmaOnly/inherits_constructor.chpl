// Verifies the behavior when a child with a default initializer inherits from
// a parent that defines a constructor.
class Parent {
  var a: int;

  // The parent defined a constructor.  Generating a default initializer for the
  // child is invalid
  proc Parent() {
    a = 11;
  }
}

pragma "use default init"
class Child : Parent {
  var b: int;
}

proc main() {
  var c = new Child(4);

  writeln(c);
  delete c;
}
