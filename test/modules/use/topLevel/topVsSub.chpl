module M {
  module N {
    proc foo() {
      writeln("In sub N.foo");
    }
  }

  use this.N;
  proc main() {
    foo();
  }
}

module N {
  proc foo() {
    writeln("In top-level N.foo");
  }
}
