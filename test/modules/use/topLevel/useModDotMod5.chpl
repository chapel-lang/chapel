module M {
  module N {
    proc foo() {
      writeln("In N.foo");
    }
  }
  proc foo() {
    writeln("In M.foo");
  }
}

module O {
  use M.N;
  proc main() {
    M.N.foo();
  }
}
