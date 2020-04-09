// Verifies the behavior when a child with a default initializer inherits from
// a parent that also had a default initializer.
class Parent {
  var a: int;
}

class Child : Parent {
  var b: int;
}

proc main() {
  var c = new borrowed Child(2, 4);
  // Note: the order of arguments in the default case is parent first, then
  // child.  This reflects the order in which fields are initialized in a
  // user-defined initializer.

  writeln(c); // So we expect this to be {a = 2, b = 4}
}
