use common;

var thisCalls: atomic int;
var localAccessCalls: atomic int;
// hijack these two methods to provide some output
inline proc _array.this(i: int) ref {
  //writeln("Custom this was called");
  thisCalls.add(1);
  return this._value.dsiAccess((i:int,));
}

inline proc _array.this(i: int, j: int) ref {
  //writeln("Custom this was called");
  thisCalls.add(1);
  return this._value.dsiAccess((i:int,j:int));
}

inline proc _array.localAccess(i: int) ref {
  //writeln("Custom localAccess was called");
  localAccessCalls.add(1);
  return this._value.dsiLocalAccess((i:int,));
}

writeln();
writeln("Starting");

{
  writeln("Slice: using slicing domain");
  var D = createDom({1..10});
  var innerD = D.expand(-1);

  var A: [D] real;
  var B: [D] real;

  ref C = B[innerD];

  B = 3;

 
  forall (i,idx) in zip(innerD, 1..) {
    A[i] =     // dynamic: no connection btw A.domain and innerD
      C[i] * idx;    // dynamic: we don't recognize arrayview creation, yet
  }
  writeln(A);
  writeln();
}

{
  writeln("Slice: using the parent domain");
  var D = createDom({1..10});
  var innerD = D.expand(-1);

  var A: [D] real;
  var B: [D] real;

  ref C = B[innerD];

  B = 3;


  forall (i,idx) in zip(A.domain, 1..) {
    if innerD.contains(i) {
      A[i] =    // static: obvious
        C[i] * idx;   // dynamic: no recognizable connection btw C.domain and A.domain
    }
  }
  writeln(A);
  writeln();
}

{ // doesn't work yet.  https://github.com/Cray/chapel-private/issues/1614
  writeln("Reindex");
  const offset = 10;
  var D = createDom({1..10});
  var DOff = createDom({1+offset..10+offset});

  var A: [D] real;
  var B: [D] real;

  ref C = B.reindex(DOff);

  B = 3;

  forall (i,idx) in zip(C.domain, 1..) {
    A[i-offset] =   // no optimization, index should be just `i`
      C[i] * idx;         // static opt: obvious
  }

  writeln(A);
  writeln();
}

{
  writeln("Rank change");
  var D = createDom({1..10, 1..10});

  var A: [D] int;
  var B: [D] int;

  B = 3;

  const colIdx = 3;

  ref C = A[1..10, colIdx];

  forall (i,idx) in zip(C.domain, 1..) {
    C[i] =            // static: obvious
      B[i, colIdx];   // no optimization, index should be just `i`
  }

  writeln(A);
  writeln();
}


writeln("`this` was called ", thisCalls , " times");
writeln("`localAccess` was called ", localAccessCalls, " times");
