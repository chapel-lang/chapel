proc f(param doRefAssign) {
  var A: [1..2] int;
  ref rA = A[1];

  A[1] = 1;
  writeln(rA);

  if doRefAssign {
    rA = 2;
  }
}

f(false);
f(true);
