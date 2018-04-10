// Verifies the behavior when a child with a default initializer inherits from
// a parent with a default initializer that inherits from a grandparent that
// defines an initializer.
class Grandparent {
  var a: int;

  // Because the grandparent defined an initializer and the initializer took no
  // arguments, we can understand how to make the super.init() call in the
  // generated parent initializer.
  proc init() {
    a = 11;
  }
}

pragma "use default init"
class Parent: Grandparent {
  var b: int;
}

pragma "use default init"
class Child : Parent {
  var c: int;
}

proc main() {
  var child = new Child(2, 4);

  writeln(child);
  delete child;
}
