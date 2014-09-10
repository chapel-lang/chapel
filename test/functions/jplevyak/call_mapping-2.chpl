
class A {
  var a : real;
  proc this(i) ref {
    return a;
  }
}

class C {
  var a : A = new A();
  var bb : int;
  proc f() {
    a(1) *= 2.0;
  }
  proc b(i) ref {
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

delete a;
delete c.a;
delete c;
