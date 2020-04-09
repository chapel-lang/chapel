class A {
  param x = 10;
}

class B: A {
  var y = x;
}

var a = new borrowed A();
var b: borrowed B?;
