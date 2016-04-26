class A {
  type myType = real;
}

class B: A {
  var x: int;
}

var a = new A();
var b: B;
var c: B;

writeln(a);

b = new B();

delete b;
delete a;
