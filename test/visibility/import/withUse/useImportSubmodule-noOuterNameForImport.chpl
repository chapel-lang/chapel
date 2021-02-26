module M {
  import this.N.{P};
  import N.P.Q; // Shouldn't work, we only brought in P
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
