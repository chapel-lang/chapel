module OuterModule {
  module M1 {

    module M2 {
      proc main() {
        foo();
      }
    }

    proc foo() {
      M2();
    }
  }

  proc M2() {
    writeln("this should be shadowed");
  }
}
