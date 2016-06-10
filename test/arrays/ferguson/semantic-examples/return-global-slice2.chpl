use samples;
//var ArrTwoThree = [new R(2), new R(3)];

var A:[1..4] R;
ref A_slice = A[2..3];
proc f() ref {
  return A_slice;
}

f() = ArrTwoThree;
writeln(A);
