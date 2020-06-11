module Outer {
  module A {
    var x: int;
    record R {
      var foo = 7;
    }
  }
  module B {
    import Outer;

    proc main() {
      writeln(Outer.A.x); // This worked today both for use and import
      var r : Outer.A.R;  // This needed fixing
      writeln(r.foo);
    }
  }
}
