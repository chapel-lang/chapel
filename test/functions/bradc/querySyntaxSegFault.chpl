class C {
  type t;
  param r: int;
}

proc foo(x: C(t=?tt, r=?rr)) {
  writeln("In foo");
}

var myC = new unmanaged C(int, 2);

foo(myC);

delete myC;
