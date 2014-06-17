class C {
  type t;
  param r: int;
}

proc foo(x: C(t=?tt, r=?rr),
        y: C(tt, rr)) {
  writeln("In foo, x = ", x, ", y = ", y);
}

var myC = new C(int, 2),
    myC2 = new C(int, 3),
    myC3 = new C(real, 4);

foo(myC, myC2);
foo(myC, myC3);
