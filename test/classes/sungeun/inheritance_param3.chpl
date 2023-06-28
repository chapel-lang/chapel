class A {
  param x = 10;
}

class B: A {
  proc foo(y=x) {
    writeln(y);
  }
}
var ownA = new owned A();
var a = ownA.borrow();
var b: borrowed B?;
