use samples;
// ArrTwoThree defined in samples as [new R(2), new R(3)]

var A:[1..4] R;
var A_slice => A[2..3];
proc f() ref {
  return A_slice;
}

f() = ArrTwoThree;
writeln(A[2].x);

// It seems clear that this program is valid and should output 2.
