class C {
  var c;
  fun f() { return c * 2; }
}
fun ff(a : C) { return a.c * 2; }

class D : C {
  var d;
  fun g() { return c * d; }
  fun f() { return c * 3; }
}
fun ff(a : D) { return ff(a:C(a.c)) * 2; }

var x = D(c = 1, d = 2.0);
var y = D(c = 3.0, d = 4);
writeln(x.c + y.d);
writeln(x.d + y.c);
writeln(x.f());
writeln(y.g());
writeln(ff(x));
writeln(ff(x:C(x.c)));
