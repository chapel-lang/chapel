module MultipleModules {
  module A {
    writeln("In A init");
    export proc bar() { writeln("in A.bar"); }
  }
  module B {
    writeln("In B init");
    export proc baz() { writeln("in B.baz"); }
  }

  use A, B, C; // to prevent them from being removed,
               // and to make their module init fns run
}
