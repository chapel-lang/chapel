use common;

var A = createArr({1..10}, int);
var B = createArr({1..10}, int);

// there must be only one static check, and two accesses optimized
forall (i, loopIdx) in zip(A.domain, 1..) with (ref A) {
  if i>1 && i<10 then
    A[i] = B[i-1] + B[i] + B[i] * loopIdx + B[i+1];
}
