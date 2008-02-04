class C {
  var c;
  def f() { return c * 2; }
}
def ff(a : C) { return a.c * 2; }

class D : C {
  var d;
  def g() { return c * d; }
  def f() { return c * 3; }
}
def ff(a : D) { return ff(a:C) * 2; }

var x = new D(c = 1, d = 2.0);
var y = new D(c = 3.0, d = 4);
writeln(x.c + y.d);
writeln(x.d + y.c);
writeln(x.f());
writeln(y.g());
writeln(ff(x));
writeln(ff(x:C));
