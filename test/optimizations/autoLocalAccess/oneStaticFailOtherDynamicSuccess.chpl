use BlockDist;

class MyClass { proc this(i) { return i; } }

// hijack these two methods to provide some output
inline proc _array.this(i: int) ref {
  writeln("Custom this was called");
  return this._value.dsiAccess((i:int,));
}

inline proc _array.localAccess(i: int) ref {
  writeln("Custom localAccess was called");
  return this._value.dsiLocalAccess((i:int,));
}

var D = newBlockDom({1..10});

var A = newBlockArr({1..10}, int); // dynamic candidate, static check is true
var B = new MyClass(); // dynamic candidate, static check is false

// we want the access to A still be through localAccess (i.e. not affected by
// the static failure of another dynamic candidate)
forall i in D {
  A[i] = B[i];
}

writeln(A);
