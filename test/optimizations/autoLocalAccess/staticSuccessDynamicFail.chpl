use BlockDist;

var A = newBlockArr({0..10}, int);
var B = newBlockArr({1..9}, int);

forall i in B.domain {
  B[i] = A[i];
}

writeln(B);
