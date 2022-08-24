class C {
  type t;
  param r: int;
}

proc foo(x: borrowed C(t=?tt, r=?rr),
         y: borrowed C(tt, rr)) {
  writeln("In foo, x = ", x, ", y = ", y);
}

var myC = (new owned C(int, 2)).borrow(),
    myC2 = (new owned C(int, 3)).borrow(),
    myC3 = (new owned C(real, 4)).borrow();

foo(myC, myC2);
foo(myC, myC3);
