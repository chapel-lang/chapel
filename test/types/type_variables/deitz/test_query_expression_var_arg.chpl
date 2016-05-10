class C {
  param p1: int;
  param p2: int;
}

var c1 = new C(2,1);
var c2 = new C(6,2);
var c3 = new C(6,3);

proc f(c: C(1+1,?p)...) {
  writeln("1: ", p(1), ": ({p1 = ", c(1).p1, ", p2 = ", c(1).p2, "})");
}

proc f(c: C(1+2+3,?p)...) {
  write("2: ", p(1), ": ({p1 = ", c(1).p1, ", p2 = ", c(1).p2, "}");
  if c.size > 1 {
    write(", {p1 = ", c(2).p1, ", p2 = ", c(2).p2, "}");
  }
  writeln(")");
}

f(c1);
f(c2);
f(c2,c3);

delete c3;
delete c2;
delete c1;
