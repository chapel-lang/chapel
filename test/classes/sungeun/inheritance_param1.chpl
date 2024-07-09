class A {
  param x = 10;
}

class B: A {
  var y = x;
}

var ownA = new owned A();
var a = ownA.borrow();
var b: borrowed B?;
