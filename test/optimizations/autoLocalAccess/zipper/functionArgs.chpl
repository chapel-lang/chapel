use BlockDist;

var D = newBlockDom({1..10});

var A: [D] int;
var B: [D] int;

// all the patterns in this test must be recognized and optimized statically

proc localQueriedDomain(a: [?d] int, b: [d] int){
  forall (i, loopIdx) in zip(a.domain, 1..) {
    a[i] += 
      b[i] * loopIdx;
  }
  writeln(a);

  forall (i, loopIdx) in zip(b.domain, 1..) {
    a[i] += 
      b[i] * loopIdx;
  }
  writeln(a);

  forall (i, loopIdx) in zip(d, 1..) {
    a[i] += 
      b[i] * loopIdx;
  }
  writeln(a);
}

proc globalDotDomain(a: [A.domain] int, b: [A.domain] int){
  forall (i, loopIdx) in zip(A.domain, 1..) {
    a[i] += 
      b[i] * loopIdx;
  }
  writeln(a);

  forall (i, loopIdx) in zip(B.domain, 1..) {
    a[i] += 
      b[i] * loopIdx;
  }
  writeln(a);

  forall (i, loopIdx) in zip(D, 1..) {
    a[i] += 
      b[i] * loopIdx;
  }
  writeln(a);
}

proc globalDomain(a: [D] int, b: [D] int){
  forall (i, loopIdx) in zip(A.domain, 1..) {
    a[i] += 
      b[i] * loopIdx;
  }
  writeln(a);

  forall (i, loopIdx) in zip(B.domain, 1..) {
    a[i] += 
      b[i] * loopIdx;
  }
  writeln(a);

  forall (i, loopIdx) in zip(D, 1..) {
    a[i] += 
      b[i] * loopIdx;
  }
  writeln(a);
}

proc main() {
  B = 10;

  localQueriedDomain(A, B);

  globalDotDomain(A, B);

  globalDomain(A, B);
}
