class C {
  param p1: int;
  param p2: int;
}

var ownC1 = new owned C(2,1);
var c1 = ownC1.borrow();
var ownC2 = new owned C(6,2);
var c2 = ownC2.borrow();

proc f(c: borrowed C(1+1,?p)) {
  writeln("1: ", p, ": {p1 = ", c.p1, ", p2 = ", c.p2, "}");
}

proc f(c: borrowed C(1+2+3,?p)) {
  writeln("2: ", p, ": {p1 = ", c.p1, ", p2 = ", c.p2, "}");
}

f(c1);
f(c2);
