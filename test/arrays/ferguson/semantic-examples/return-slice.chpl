proc f() {
  var A:[1..4] int;
  return A[2..3];
}

var A_slice = f();
writeln(A_slice);
