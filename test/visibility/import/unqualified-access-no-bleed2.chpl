module M {
  var x: int;
  proc foo() {
    writeln("In M.foo()");
  }
}
module U {
  import M.x;

  proc main() {
    foo(); // foo wasn't imported, shouldn't be able to see it
  }
}
