module Outer {
  module A {
    record R {

      proc type foo() {
        writeln("In type method");
      }
    }
  }

  module User {
    use Outer;

    proc main() {
      [i in 0..3] A.R.foo();
    }
  }
}
