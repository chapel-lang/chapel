use common;

var A = createArr({0..10}, int);
var B = createArr({0..10}, int);

// hijack these two methods to provide some output
inline proc _array.this(i: int) ref {
  writeln("Custom this was called");
  return this._value.dsiAccess((i:int,));
}

inline proc _array.localAccess(i: int) ref {
  writeln("Custom localAccess was called");
  return this._value.dsiLocalAccess((i:int,));
}

B = 3;

// A should be optimized statically
// B should be optimized dynamically
forall i in A.domain {
  A[i] +=
    B[i];
}

writeln(A);

var Dom = createDom({0..10});

// A should be optimized dynamically
// B should be optimized dynamically
forall i in Dom {
  A[i] +=
    B[i];
}

writeln(A);
