class A {
  param x = 10;
}

class B: A {
  var y: int;
}

var a = new A();
writeln("{x = ", a.x, "}");
var b: B;
