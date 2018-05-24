class C {
  type t;
  param r: int;
}

proc foo(x: C(t=?tt, r=?rr),
        y: C(tt, rr)) {
  writeln("In foo-a, x = {r = ", x.r, "}, y = {r = ", y.r, "}");
}

proc foo(x: C(t=?tt, r=?rr),
        y: C(t=?tt2, r=?rr2)) {
  writeln("In foo-b, x = {r = ", x.r, "}, y = {r = ", y.r, "}");
}

var myC = new borrowed C(int, 2),
    myC2 = new borrowed C(int, 2),
    myC3 = new borrowed C(real, 3);

foo(myC, myC2);
foo(myC, myC3);
