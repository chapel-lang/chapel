module OuterModule {
  module M {
    proc foo() {
      writeln("In foo");
    }
  }

  module M2 {
    config param qualifiedAccess = false;

    use M except *;  // require all symbols in M to be fully-qualified.

    proc main() {
      if qualifiedAccess then
        M.foo();
      else
        foo();
    }
  }
}
