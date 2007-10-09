class C {
  var x: [1..3] int;
  def foo() var {
    writeln(setter);
    return x;
  }
}

var c = C();

c.foo() = 2;
writeln(c.foo());
