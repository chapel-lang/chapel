use common;

var A = createArr({0..10}, int);
var B = createArr({1..9}, int);

forall i in B.domain with (ref B) {
  B[i] = A[i];
}

writeln(B);
