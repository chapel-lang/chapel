module M {
  private var x = 42;

  module N {
    module O {
      import super.super.x;

      proc main() {
        writeln(x);
      }
    }
  }
}