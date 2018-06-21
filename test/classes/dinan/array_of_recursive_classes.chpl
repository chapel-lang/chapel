class A {
  var x: int;
  var b: [1..x] unmanaged B;
}

class B {
  var a: unmanaged A;   // This produces an error
//var a: A(); // This works
}


var b = new unmanaged B();

b.a = new unmanaged A(10);

writeln(b.a.x);

delete b.a;
delete b;
