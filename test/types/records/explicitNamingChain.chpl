module Outer {
  module A {
    record R1 {
      var x1: int;
    }
    proc R1.func() {
      writeln("I'm a method");
    }
  }
  module B {
    private use Outer;
    record R2 {
      var x2: A.R1;
    }
  }
  module C {
    use Outer;

    proc main() {
      var r2: B.R2;
      r2.x2.func();
    }
  }
}
