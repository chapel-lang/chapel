module M {
  module N {
    proc foo() {
      writeln("In sub N.foo");
    }
  }

  proc main() {
    use this.N;
    foo();
  }
}

module N {
  proc foo() {
    writeln("In top-level N.foo");
  }
}
