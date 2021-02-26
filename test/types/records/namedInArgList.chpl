module Outer {
  module A {
    record R {
      var x = 11;
    }
  }

  module User {
    import Outer;

    proc foo(blah: Outer.A.R) {
      writeln(blah.x);
    }

    proc main() {
      var r: Outer.A.R;
      foo(r);
    }
  }
}
