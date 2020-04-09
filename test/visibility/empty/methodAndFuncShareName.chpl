module OuterModule {
  module M {
    class Foo {
      proc sameName() {
        writeln("in the method");
      }
    }

    proc sameName() {
      writeln("in the function");
    }
  }

  proc main() {
    use M only;

    sameName();
  }
}
