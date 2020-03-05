module A {
  private use Inner;

  class Foo {
    var x: int;
    param p = 4;
    type t = int;
    const c: bool;
  }
  private module Inner {
    proc Foo.hidden() {
      writeln("Uh oh, found the hidden module!");
    }
  }
}

module B {
  import A;

  proc main() {
    var foo = new A.Foo(3, 6, real, true);
    foo.hidden();
  }
}
