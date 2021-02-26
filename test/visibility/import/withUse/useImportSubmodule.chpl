module M {
  use N;
  import N;  // fails even though `use N` should bring `N` into scope
  N.foo();
  module N {
    proc foo() {
      writeln("foo");
    }
  }
}
