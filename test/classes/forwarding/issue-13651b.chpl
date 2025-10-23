class B {
    proc b1() { writeln("B.b1"); }
    proc b2() { writeln("B.b2"); }
}
class A {
  var b : unmanaged B;
  forwarding b;
}

var a = new owned A(new unmanaged B());
{
  a.b1();
  a.b2();
}
