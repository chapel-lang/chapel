class C {
  param p1: int;
  param p2: int;
}

var c1Obj = new C(2,1); var c1 = c1Obj.borrow();
var c2Obj = new C(6,2); var c2 = c2Obj.borrow();
var c3Obj = new C(9,3); var c3 = c3Obj.borrow();

proc f(c: borrowed C(1+1,?p)) {
  writeln("1: ", p, ": ", c);
}

proc f(c: borrowed C(1+2+3,?p)) {
  writeln("2: ", p, ": ", c);
}

f(c1);
f(c2);
f(c3);
