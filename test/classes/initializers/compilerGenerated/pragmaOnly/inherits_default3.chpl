// Verifies the behavior when a child with a default initializer inherits from
// a parent that also had a default initializer.
pragma "use default init"
class Parent {
  var a: int;
}

class Child : Parent {
  var b: int;
}

proc main() {
  var c = new Child(4, 2);
  // Note: the order of arguments in the default case is child first, then
  // parent.  This reflects the order in which fields are initialized in a
  // user-defined initializer.

  writeln(c); // So we expect this to be {a = 2, b = 4}
  delete c;
}
