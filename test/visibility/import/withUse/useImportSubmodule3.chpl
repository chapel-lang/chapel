module M {
  import this.N.{P};
  // Used to fail even though `import this.N.{P}` should bring `P` into scope
  import P.Q;
  Q.foo();
  module N {
    module P {
      module Q {
        proc foo() {
          writeln("foo");
        }
      }
    }
  }
}
