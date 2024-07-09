use common;

class MyClass { proc this(i) { return i; } }

var D = createDom({1..10});

var A = createArr({1..10}, int); // dynamic candidate, static check is true
var B = new MyClass(); // dynamic candidate, static check is false

// we want the access to A still be through localAccess (i.e. not affected by
// the static failure of another dynamic candidate)
forall i in D with (ref A) {
  A[i] = B[i];
}

writeln(A);
