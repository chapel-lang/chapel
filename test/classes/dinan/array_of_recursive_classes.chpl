class A {
  var x: int;
  var b: [1..x] unmanaged B?;
}

class B {
  var a: unmanaged A?;   // a recursive class
}


var b = new B();

b.a = new unmanaged A(10);

writeln(b.a!.x);

delete b.a;
