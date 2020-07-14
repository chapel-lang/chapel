use BlockDist;

// hijack these two methods to provide some output
inline proc _array.this(i: int) ref {
  writeln("Custom this was called");
  return this._value.dsiAccess((i:int,));
}

inline proc _array.localAccess(i: int) ref {
  writeln("Custom localAccess was called");
  return this._value.dsiLocalAccess((i:int,));
}


var A = newBlockArr({1..10}, int);

for (a,i) in zip(A, A.domain) do a=i;

forall a in A {
  A[a] = a*2;
}

writeln(A);
