class C {
  type t;
  param r: int;
}

proc foo(x: borrowed C(t=?tt, r=?rr),
         y: borrowed C(tt, rr)) {
  writeln("In foo, x = ", x, ", y = ", y);
}
var cObj = new C(int, 2), c2Obj = new C(int, 3), c3Obj = new C(real, 4);
var myC = cObj.borrow(),
    myC2 = c2Obj.borrow(),
    myC3 = c3Obj.borrow();

foo(myC, myC2);
foo(myC, myC3);
