class A {
  param x = 10;
}

class B: A {
  var y = x;
}

var a = (new owned A()).borrow();
var b: borrowed B?;
