// Verifies the behavior when a child with a default initializer inherits from
// a parent with a default initializer that inherits from a grandparent that
// defines an initializer that we won't necessarily know what to do to call.
class GrandParent {
  var a: int;

  // Because the grandparent defined an initializer and the initializer took
  // some arguments, we can't make any assumptions when creating the
  // super.init() call in the generated parent initializer.
  proc init(x1: int, x2: int) {
    a = x1 + x2;
  }
}


class Parent: GrandParent {
  var b: int;
}

class Child : Parent {
  var c: int;
}

proc main() {
  var child = new owned Child(4);
  // TODO: I would like a better error message, this current one is confusing
  // (but reasonable when you know why it is complaining)

  writeln(child);
}
