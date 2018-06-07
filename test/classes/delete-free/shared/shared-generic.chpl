class GenericClass {
  type t;
  var v: int;
}

proc f(x: shared GenericClass) {
  writeln(x);
}

var a = new shared GenericClass(int, 3);
var b = new shared GenericClass(real, 4);
f(a);
f(b);
