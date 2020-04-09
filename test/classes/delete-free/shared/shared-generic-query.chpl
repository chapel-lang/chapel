class GenericClass {
  type t;
  var v: int;
}

proc f(x: shared GenericClass(?t)) {
  writeln(t:string, " ", x);
}

var a = new shared GenericClass(int, 3);
var b = new shared GenericClass(real, 4);
f(a);
f(b);
