use BlockCycDist;

config const n = 10, blkSize=8;

proc main() {
  const MatVecSpace = {1..n, 1..n+1} dmapped BlockCyclic(startIdx=(1,1), (8,8));
  var Ab: [MatVecSpace] real;

  foo(Ab[.., n+1..n+1]);

  writeln(Ab);
}


proc foo(A: [?yD]) {
  A[n+1,-4] = 1.0;
}
