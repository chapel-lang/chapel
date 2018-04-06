// Verifies the behavior when a child with a default initializer inherits from
// a parent that also had a default initializer and inherited from a parent that
// also had a default initializer.
pragma "use default init"
class GrandParent {
  var a: int;
}

pragma "use default init"
class Parent : GrandParent {
  var b: int;
}

pragma "use default init"
class Child : Parent {
  var c: int;
}

proc main() {
  var child = new Child(1, 2, 4);
  // Note: the order of arguments in the default case is parent first, then
  // child.  This reflects the order in which fields are initialized in a
  // user-defined initializer.

  writeln(child); // So we expect this to be {a = 1, b = 2, c = 4}
  delete child;
}
