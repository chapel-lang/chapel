// Verifies the behavior when a child with a default initializer inherits from
// a parent with a default initializer that inherits from a grandparent that
// defines a constructor.
class GrandParent {
  var a: int;

  // The grandparent defined a constructor.  Generating a default initializer
  // for the parent and child is invalid
  proc GrandParent() {
    a = 11;
  }
}

class Parent: GrandParent {
  var b: int;
}

class Child : Parent {
  var c: int;
}

proc main() {
  var child = new Child(4);

  writeln(child);
  delete child;
}
