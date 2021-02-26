module Outer {
  module A {
    var x: int;
    record R {
      var foo = 7;
    }
  }
  module B {
    use Outer;

    proc main() {
      writeln(A.x); // This worked
      var r : A.R;  // This needed fixing
      writeln(r.foo);
    }
  }
}
