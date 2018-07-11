class Foo {
  const D : domain(2);
  const A : [D] int = 10;

  proc init(n : int) {
    D = { 1 .. n, 1 .. n };

  }
}

var a = new unmanaged Foo(10);

writeln(a);

delete a;
