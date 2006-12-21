def lutx(A: []) {
  const n = A.diminfo(1).size;

  if (A.rank != 2) then halt("lutx requires a 2D array");
  if (A.diminfo(2).size != n) then halt("lutx requires a square input array");

  var p: [1..n] index(A)