function f(x, y, z) { writeln(x + y + z); return x + y + z; }
class C {
  var a;
}
var c = C(=> f(1));
var d = => c.a(2);
d(3);
