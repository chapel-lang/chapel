use common;

var A = createArr({0..10}, int);
var B = createArr({1..9}, int);

forall (i, loopIdx) in zip(B.domain, 1..) {
  B[i] = A[i] * loopIdx;
}

writeln(B);
