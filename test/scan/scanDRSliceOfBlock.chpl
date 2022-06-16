use BlockDist;
var A = newBlockArr({1..10}, int);
forall i in A.domain do
  A[i] = i;
ref Aview = A[{2..9}];
var B = + scan Aview;
writeln(B);
