module OuterModule {
  module M {
    class Foo {
      proc primaryMethod() {
        writeln("in primary method");
      }
    }

    proc Foo.secondaryMethod() {
      writeln("in secondary method");
    }
  }

  module M2 {
    config param accessPrimary = false;

    use M only; // require all symbols in M to be fully-qualified

    proc main() {
      var foo = new borrowed M.Foo();
      if (accessPrimary) then
        foo.primaryMethod();
      else
        foo.secondaryMethod();
    }
  }
}
