class C {
  type t;
  param r: int;
}

proc foo(x: borrowed C(t=?tt, r=?rr),
         y: borrowed C(tt, rr)) {
  writeln("In foo-a, x = {r = ", x.r, "}, y = {r = ", y.r, "}");
}

proc foo(x: borrowed C(t=?tt, r=?rr),
         y: borrowed C(t=?tt2, r=?rr2)) {
  writeln("In foo-b, x = {r = ", x.r, "}, y = {r = ", y.r, "}");
}

var myC = new borrowed C(int, 2),
    myC2 = new borrowed C(int, 2),
    myC3 = (new owned C(real, 3)).borrow();

foo(myC, myC2);
foo(myC, myC3);
