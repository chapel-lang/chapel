module Outer {
  module A {
    record R {
      var x = 11;
    }
  }

  module User {
    import Outer;

    proc bar() {
      // Verifies that the function can still explicitly name the type, even
      // if it's a nested function
      proc foo(blah: Outer.A.R) {
        writeln(blah.x);
      }
      {
        var r: Outer.A.R;
        foo(r);
      }
    }

    proc main() {
      bar();
    }
  }
}
