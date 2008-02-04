class C {
  def foo(x) {
    writeln("In foo, x is: ", x);
  }
}

var c = new C();

c.foo(2);
(c.foo)(3);
