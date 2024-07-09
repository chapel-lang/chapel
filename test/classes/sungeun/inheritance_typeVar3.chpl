class A {
  type myType = real;
}

class B: A {
  proc foo(x: myType) {
    writeln(x);
  }
}
var ownA = new owned A();
var a = ownA.borrow();
var b: borrowed B?;
