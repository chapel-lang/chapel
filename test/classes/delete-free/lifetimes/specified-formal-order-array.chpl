class C { var x: int; }

proc setA(ref lhs, rhs) lifetime lhs < rhs {
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
