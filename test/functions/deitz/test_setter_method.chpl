class C {
  var x: int;
  def foo() var {
    writeln(setter);
    return x;
  }
}

var c = new C();

c.foo() = 2;
writeln(c.foo());
