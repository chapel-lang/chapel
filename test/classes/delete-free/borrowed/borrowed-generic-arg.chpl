class GenericClass {
  type t;
  var v: int;
}

proc f(x: borrowed GenericClass(?)) {
  writeln(x);
}

var oa = new owned GenericClass(int, 3);
var ob = new owned GenericClass(real, 4);

var a = oa.borrow();
var b = ob.borrow();
f(a);
f(b);
