var A:[1..4] int;
ref A_slice = A[2..3];
proc f() ref {
  return A_slice;
}

f() = 1;
writeln(A);
