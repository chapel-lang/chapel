class C {
  def foo(x) {
    writeln("In foo, x is: ", x);
  }
}

var c = C();

c.foo(2);
(c.foo)(3);
