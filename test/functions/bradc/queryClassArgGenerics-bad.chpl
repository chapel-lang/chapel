class C {
  type t;
  param r: int;
}

proc foo(x: borrowed C(t=?tt, r=?rr),
         y: borrowed C(tt, rr)) {
  writeln("In foo, x = ", x, ", y = ", y);
}

var myC = new borrowed C(int, 2),
    myC2 = new borrowed C(int, 3),
    myC3 = new borrowed C(real, 4);

foo(myC, myC2);
foo(myC, myC3);
