class C {
  param p1: int;
  param p2: int;
}

var c1 = (new owned C(2,1)).borrow();
var c2 = (new owned C(6,2)).borrow();
var c3 = (new owned C(9,3)).borrow();

proc f(c: borrowed C(1+1,?p)) {
  writeln("1: ", p, ": ", c);
}

proc f(c: borrowed C(1+2+3,?p)) {
  writeln("2: ", p, ": ", c);
}

f(c1);
f(c2);
f(c3);
