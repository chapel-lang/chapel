// Verifies the behavior when a child with a default initializer inherits from
// a parent that defines an initializer that we won't necessarily know what to
// do to call.
class Parent {
  var a: int;

  // Because the parent defined an initializer and the initializer took any
  // arguments, we can't make any assumptions when creating the super.init()
  // call in the generated child initializer.
  proc init(x1: int, x2: int) {
    a = x1 + x2;

  }
}

class Child : Parent {
  var b: int;
}

proc main() {
  var c = new shared Child(4);
  // TODO: I would like a better error message, this current one is confusing
  // (but reasonable when you know why it is complaining)

  writeln(c);
}
