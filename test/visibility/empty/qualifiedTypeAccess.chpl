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

    use super.M only; // require all symbols in M to be fully-qualified

    proc main() {
      var ownFoo = new owned M.Foo();
      var foo = ownFoo.borrow();
      if (accessPrimary) then
        foo.primaryMethod();
      else
        foo.secondaryMethod();
    }
  }
}
