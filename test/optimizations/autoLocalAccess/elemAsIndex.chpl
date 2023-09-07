use BlockDist;

var A = blockDist.createArray({1..10}, int);

for (a,i) in zip(A, A.domain) do a=i;

forall a in A with (ref A) {
  A[a] = a*2;
}

writeln(A);
