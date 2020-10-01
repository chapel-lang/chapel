// The forall here cannot be optimized. But we must make sure that we are not
// doing anything funny with the iterator to make that decision

// we must have exactly one iterator invocation (the automatic localaccess
// optimization shouldn't create another iterator record)
iter myIter() {
  writeln("serial iterator invoked");
  for i in 1..10 do yield i;
}

iter myIter(param tag: iterKind) where tag == iterKind.standalone {
  writeln("standalone iterator invoked");
  coforall t in 1..1 {
    for i in 1..10 do yield i;
  }
}

iter myIter(param tag: iterKind) where tag == iterKind.leader {
  writeln("leader iterator invoked");
  yield (1..10, );
}

iter myIter(param tag: iterKind, followThis) where tag == iterKind.follower {
  writeln("follower iterator invoked");
  for i in followThis[0] do yield i;
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

var A: [1..10] int;
var B: [1..10] int;
var C: [1..10] int;

B = 1;
C = 2;

forall i in myIter() {
  A[i] = 
    B[i] + 
    C[i];
}

writeln(A);
