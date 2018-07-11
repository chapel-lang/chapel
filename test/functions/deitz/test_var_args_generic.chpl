class C {
  param rank: int;

  proc foo(i ...rank) {
    writeln("wrong");
  }

  proc foo(i: int) {
    writeln("right");
  }
}

var c = new unmanaged C(1);

c.foo(1);

delete c;
