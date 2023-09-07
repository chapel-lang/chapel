use common;

var A = createArr({0..10}, int);
var B = createArr({0..10}, int);

B = 3;

// A should be optimized statically
// B should be optimized dynamically
forall i in A.domain with (ref A) {
  A[i] +=
    B[i];
}

writeln(A);

var Dom = createDom({0..10});

// A should be optimized dynamically
// B should be optimized dynamically
forall i in Dom with (ref A) {
  A[i] +=
    B[i];
}

writeln(A);
