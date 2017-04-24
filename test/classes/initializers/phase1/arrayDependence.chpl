class Foo {
  const D:domain(2);
  const A:[D] int;

  proc init(n:int) {
    D = {1..n, 1..n};
    super.init();
  }
}


var a = new Foo(10);

writeln(a);

delete a;
