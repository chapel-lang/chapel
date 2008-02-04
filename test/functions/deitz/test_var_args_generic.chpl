class C {
  param rank: int;

  def foo(i ...rank) {
    writeln("wrong");
  }

  def foo(i: int) {
    writeln("right");
  }
}

var c = new C(1);

c.foo(1);
