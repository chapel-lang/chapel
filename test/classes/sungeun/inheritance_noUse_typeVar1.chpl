class A {
  type myType = real;
}

class B: A {
  var x: int;
}

var a = new unmanaged A();
var b: unmanaged B?;
var c: unmanaged B?;

writeln(a);

b = new unmanaged B();

delete b;
delete a;
