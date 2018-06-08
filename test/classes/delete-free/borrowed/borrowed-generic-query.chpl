class GenericClass {
  type t;
  var v: int;
}

proc f(x: borrowed GenericClass(?t)) {
  writeln(t:string, " " , x);
}

var a = new borrowed GenericClass(int, 3);
var b = new borrowed GenericClass(real, 4);
f(a);
f(b);
