class C {
  type t;
  param r: int;
}

proc foo(x: C(t=?tt, r=?rr),
        y: C(tt, rr)) {
  writeln("In foo-a, x = ", x, ", y = ", y);
}

proc foo(x: C(t=?tt, r=?rr),
        y: C(t2=?tt, r2=?rr)) {
  writeln("In foo-b, x = ", x, ", y = ", y);
}

var myC = new C(int, 2),
    myC2 = new C(int, 2),
    myC3 = new C(real, 3);

foo(myC, myC2);
foo(myC, myC3);
