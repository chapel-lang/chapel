
class A {
  var a : real;
  def this(i) var {
    return a;
  }
}

class C {
  var a : A = new A();
  var bb : int;
  def f() {
    a(1) *= 2.0;
  }
  def b(i) var {
    return bb; 
  }
}

var a = new A();
a(1) = 1.0;
writeln(a(1));

var c = new C();

c.a(1) = 1.0;
c.f();
writeln(c.a(1));

c.b(1) = 2;
writeln(c.b(1));
