module M {
  use N;
  import N.P;  // fails even though `use N` should bring `N` into scope         
  P.foo();
  module N {
    module P {
      proc foo() {
	writeln("foo");
      }
    }
  }
}
