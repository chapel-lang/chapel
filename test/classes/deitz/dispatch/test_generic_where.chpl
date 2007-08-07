class C {
  var x: int;
  def foo(param rank: int) {
    writeln("foo<", rank, ">: (x = ", x, ")");
  }
}

class D: C {
  var y: int;
  def foo(param rank: int) where rank == 1 {
    writeln("foo<", rank, ">: (x = ", x, ", y = ", y, ")");
  }
}

var c = C();
c.foo(1);
c.foo(2);

c = D();
c.foo(1);
c.foo(2);
