use BlockDist;
var A = blockDist.createArray({1..10}, int);
forall i in A.domain with (ref A) do
  A[i] = i;
ref Aview = A[{2..9}];
var B = + scan Aview;
writeln(B);
