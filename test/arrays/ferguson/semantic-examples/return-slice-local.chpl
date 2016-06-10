use myrecord;
proc f() {
  var A:[1..4] R;
  return A[2..3];
}

var A_slice = f();
writeln(A_slice);
