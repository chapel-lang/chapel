private module A {
  class Foo {
    var x: int;
    param p = 4;
    type t = int;
    const c: bool;
  }
}

module B {
  import A;

  proc main() {
    var foo = new A.Foo(3, 6, real, true);
    writeln(foo.x);
    writeln(foo.p);
    writeln(foo.t: string);
    writeln(foo.c);
  }
}
