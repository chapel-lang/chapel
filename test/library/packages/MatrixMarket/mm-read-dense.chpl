use MatrixMarket;

config const fname = "sparse.mtx";

var A = mmread(real, fname);

var (lowx, lowy) = A.domain.low;
var (highx, highy) = A.domain.high;

writeln("a ", 1+highx-lowx, "x", 1+highy-lowy, " matrix");
var Copy:[ lowx..highx, lowy..highy ] real;
for ij in Copy.domain {
  Copy[ij] = A[ij];
}

writeln(Copy);
