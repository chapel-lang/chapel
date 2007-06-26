class C {
  var x: int;
  def foo() {
    writeln("foo: (x = ", x, ")");
  }
}

class D: C {
  param rank: int;
  var y: int;
  def foo() where rank == 1 {
    writeln("foo<", rank, ">: (x = ", x, ", y = ", y, ")");
  }
}

var c = C();
c.foo();
c.foo();

c = D(rank=1);
c.foo();
c.foo();

c = D(rank=2);
c.foo();
c.foo();
