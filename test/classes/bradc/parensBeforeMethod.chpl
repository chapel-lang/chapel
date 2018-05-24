class C {
  proc foo(x) {
    writeln("In foo, x is: ", x);
  }
}

var c = new borrowed C();

c.foo(2);
(c.foo)(3);
