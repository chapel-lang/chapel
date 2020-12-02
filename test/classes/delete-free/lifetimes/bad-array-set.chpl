class C { var x: int; }

proc bad1() {
  var B: [1..2] borrowed C?;
  {
    var A: [1..2] owned C?;
    A[1] = new owned C(1);
    A[2] = new owned C(2);
    B[1] = A[1].borrow();
    B[2] = A[2].borrow();
  }
  writeln(B[1]!.x);
}
bad1();

proc bad2() {
  var B: [1..2] borrowed C?;
  {
    var A: [1..2] owned C?;
    A[1] = new owned C(1);
    A[2] = new owned C(2);
    var Ab: [1..2] borrowed C?;
    Ab[1] = A[1].borrow();
    Ab[2] = A[2].borrow();

    B[1] = Ab[1];
    B[2] = Ab[2];
  }
  writeln(B[1]!.x);
}
bad2();
