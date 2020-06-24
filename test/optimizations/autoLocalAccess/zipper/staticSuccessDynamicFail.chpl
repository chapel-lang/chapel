use BlockDist;

var A = newBlockArr({0..10}, int);
var B = newBlockArr({1..9}, int);

forall (i, loopIdx) in zip(B.domain, 1..) {
  B[i] = A[i] * loopIdx;
}

writeln(B);
