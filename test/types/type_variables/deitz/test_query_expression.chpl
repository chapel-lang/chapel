class C {
  param p1: int;
  param p2: int;
}

var c1 = new borrowed C(2,1);
var c2 = new borrowed C(6,2);

proc f(c: borrowed C(1+1,?p)) {
  writeln("1: ", p, ": {p1 = ", c.p1, ", p2 = ", c.p2, "}");
}

proc f(c: borrowed C(1+2+3,?p)) {
  writeln("2: ", p, ": {p1 = ", c.p1, ", p2 = ", c.p2, "}");
}

f(c1);
f(c2);
