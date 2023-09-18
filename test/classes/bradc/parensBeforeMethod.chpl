class C {
  proc foo(x) {
    writeln("In foo, x is: ", x);
  }
}

var ownC = new owned C();
var c = ownC.borrow();

c.foo(2);
(c.foo)(3);
