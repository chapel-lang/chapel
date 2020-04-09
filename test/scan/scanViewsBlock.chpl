use BlockDist;

proc scanArr(X) {
  var Y = + scan X;
  writeln(Y, ": [", Y.domain, "]");
}

var A: [{1..10} dmapped Block({1..10})] int = [i in 1..10] i;

scanArr(A);
scanArr(A[3..5]);
scanArr(A.reindex(0..9));
scanArr(A[3..5].reindex(-1..1));
scanArr(A.reindex(0..9)[0..2]);

var B: [1..3, 1..3] int = [(i,j) in {1..3, 1..3}] i*10 + j;

scanArr(B[2, 1..3]);
scanArr(B[1..3, 3]);
