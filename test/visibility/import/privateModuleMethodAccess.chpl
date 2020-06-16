module A {
  private use Inner;

  class Foo {
    var x: int;
    param p = 4;
    type t = int;
    const c: bool;
  }
  private module Inner {
    import A.Foo;
    proc Foo.hidden() {
      writeln("Found the hidden module!");
    }
  }

  proc Foo.callHidden() {
    hidden();
  }
}

module B {
  import A;

  proc main() {
    var foo = new A.Foo(3, 6, real, true);
    foo.callHidden();
  }
}
