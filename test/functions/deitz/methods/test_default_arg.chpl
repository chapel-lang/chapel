class C {
  var x : int = 2;
  def foo(y : int = 4) {
    writeln(y, " and ", x);
  }
}

var c = new C();
writeln(c);
c.foo(3);
c.foo();
