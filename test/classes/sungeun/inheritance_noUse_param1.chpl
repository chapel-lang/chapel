class A {
  param x = 10;
}

class B: A {
  var y: int;
}

var a = new A();
var b: B;
var c: B;

writeln(a);
writeln("{x = ", a.x, "}");

b = new B();

delete b;
delete a;

