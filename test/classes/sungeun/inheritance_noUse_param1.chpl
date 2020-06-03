class A {
  param x = 10;
}

class B: A {
  var y: int;
}

var a = new owned A();
var b: owned B?;
var c: owned B?;

writeln(a);
writeln("{x = ", a.x, "}");

b = new owned B();
