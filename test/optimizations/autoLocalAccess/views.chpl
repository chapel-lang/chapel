use common;

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

 
  forall i in innerD with (ref A) {
    A[i] =     // dynamic: no connection btw A.domain and innerD
      C[i];    // dynamic: we don't recognize arrayview creation, yet
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


  forall i in A.domain with (ref A) {
    if innerD.contains(i) {
      A[i] =    // static: obvious
        C[i];   // dynamic: no recognizable connection btw C.domain and A.domain
    }
  }
  writeln(A);
  writeln();
}

{ // doesn't work yet.  https://github.com/Cray/chapel-private/issues/1620
  writeln("Reindex");
  const offset = 10;
  var D = createDom({1..10});
  var DOff = createDom({1+offset..10+offset});

  var A: [D] real;
  var B: [D] real;

  ref C = B.reindex(DOff);

  B = 3;

  forall i in C.domain with (ref A) {
    A[i-offset] =   // no optimization, index should be just `i`
      C[i];         // static opt: obvious
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

  forall i in C.domain with (ref C) {
    C[i] =            // static: obvious
      B[i, colIdx];   // no optimization, index should be just `i`
  }

  writeln(A);
  writeln();
}
