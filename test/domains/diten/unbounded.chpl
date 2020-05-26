proc test(r) {
  var D = {r};
  var A = for i in 0..15 do i;
  writeln(D);
  ref rAD = A[D];
  writeln(rAD);
}

test(1..);
test(..10);
test(..);
test(1..10);
