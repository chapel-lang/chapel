use common;

// we should be able to optimize the forall here by calling this function only
// once (no extra calls for the optimization)
proc returnDom() {
  writeln("returnDom is called");
  return createDom({1..10});
}

var A = createArr({1..10}, int);
var B = createArr({1..10}, int);
var C = createArr({1..10}, int);

B = 1;
C = 2;

forall i in returnDom() with (ref A) {
  A[i] =
    B[i] + 
    C[i];
}

writeln(A);
