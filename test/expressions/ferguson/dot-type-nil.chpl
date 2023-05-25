class C {
  type t;
  var x: t;
}

var c1: C(int) = (new owned C(int)).borrow();
var c2: C(real) = (new owned C(real)).borrow();

writeln(c1.x.type: string);
writeln(c2.x.type: string);
