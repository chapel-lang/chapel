// Verifies the behavior when a child with a default initializer inherits from
// a parent that defines an initializer.
class Parent {
  var a: int;

  // Because the parent defined an initializer and the initializer took no
  // arguments, we can understand how to make the super.init() call in the
  // generated child initializer.
  proc init() {
    a = 11;
  }
}

class Child : Parent {
  var b: int;
}

proc main() {
  var c = new owned Child(4);

  writeln(c);
}
