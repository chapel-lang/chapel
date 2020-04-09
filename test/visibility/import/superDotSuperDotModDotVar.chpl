module M {
  module N {
    var x = 42;

    module O {
      import super.super.N.x;

      proc main() {
        writeln(x);
      }
    }
  }
}