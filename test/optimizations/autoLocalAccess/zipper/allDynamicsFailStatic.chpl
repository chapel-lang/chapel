use common;
use CyclicDist;

var D = createDom({1..10});


var A: [D] int;
var B = newCyclicArr({1..10}, int);

// A is a static candidate, and will pass the static check
// B is a dynamic candidate that'll fail dynamic check, in that case, we want
// the dynamic-checked loop copy to be folded out. In other words,
// post-resolution we should have only one instance of this loop

// One way to confirm that is that "Static check successful" message is shown
// only once in the compiler logs
forall (i, loopIdx) in zip(D, 1..) {
  A[i] = 
    B[i]*loopIdx;
}
