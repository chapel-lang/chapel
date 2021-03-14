module M {
  proc foo() {
    writeln("In M.foo()");
  }
}
module U {
  import M.foo;

  proc main() {
    foo();
  }
}
