module M {
  proc foo() {
    writeln("In M.foo()");
  }
}
module U {
  import M.foo;

  proc main() {
    // Import statements of a single symbol in a module should not enable
    // qualified access of that symbol (or any other)
    M.foo();
  }
}
