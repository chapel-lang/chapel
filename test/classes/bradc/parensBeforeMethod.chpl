class C {
  proc foo(x) {
    writeln("In foo, x is: ", x);
  }
}

var c = (new owned C()).borrow();

c.foo(2);
(c.foo)(3);
