module Outer {
  module A {
    record R {
      var x: int;
    }
  }
  module User {
    use Outer;

    record R2 {
      var a: A.R;
    }

    proc main() {
      var r2: R2;
      writeln(r2);
    }
  }
}
