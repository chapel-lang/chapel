def f(x, y, z) { writeln(x + y + z); return x + y + z; }
class A {
  var a;
  def this(x,y) { return a(x, y); }
}
class C {
  var a;
}
var c = A(=> f(1));
var d = C(c);
var e = => d.a(2);
e(3);
