use common;

// we should be able to optimize the forall here by calling this function only
// once (no extra calls for the optimization)
proc returnDom() {
  writeln("returnDom is called");
  return createDom({1..10});
}

// hijack these two methods to provide some output
inline proc _array.this(i: int) ref {
  writeln("Custom this was called");
  return this._value.dsiAccess((i:int,));
}

inline proc _array.localAccess(i: int) ref {
  writeln("Custom localAccess was called");
  return this._value.dsiLocalAccess((i:int,));
}


var A = createArr({1..10}, int);
var B = createArr({1..10}, int);
var C = createArr({1..10}, int);

B = 1;
C = 2;

forall i in returnDom() {
  A[i] = 
    B[i] + 
    C[i];
}

writeln(A);
