class C { var x: int; }

// This function should require lifetime(lhs) < lifetime(rhs)
proc setC(ref lhs: C?, rhs: C) {
  lhs = rhs;
}

proc bad1() {
  var b: borrowed C?;
  {
    var own = new owned C();
    setC(b, own.borrow());
  }
  writeln(b!.x);
}
bad1();

// This function should require lifetime(lhs) == lifetime(rhs)
proc swapC(ref lhs: C?, ref rhs: C?) {
  var tmp = rhs;
  rhs = lhs;
  lhs = tmp;
}

proc bad2() {
  var b: borrowed C?;
  {
    var own = new owned C();
    var bb: borrowed C? = own.borrow();
    swapC(b, bb);
  }
  writeln(b!.x);
}
bad2();

// This function should require lifetime(lhs) < lifetime(rhs)
proc setA(ref lhs, rhs) {
  lhs[1] = rhs[1];
  lhs[2] = rhs[2];
}

proc bad3() {
  var B: [1..2] borrowed C?;
  {
    var A: [1..2] owned C?;
    A[1] = new owned C(1);
    A[2] = new owned C(2);
    var Ab: [1..2] borrowed C?;
    Ab[1] = A[1].borrow();
    Ab[2] = A[2].borrow();
    setA(B, Ab);
  }
  writeln(B[1]!.x);
}
bad3();
