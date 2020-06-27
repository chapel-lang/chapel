use BlockDist;

var D = newBlockDom({1..10});

var A: [D] int;
var B: [D] int;

// all the patterns in this test must be recognized and optimized statically

proc localQueriedDomain(a: [?d] int, b: [d] int){
  forall i in a.domain {
    a[i] += 
      b[i];
  }
  writeln(a);

  forall i in b.domain {
    a[i] += 
      b[i];
  }
  writeln(a);

  forall i in d {
    a[i] += 
      b[i];
  }
  writeln(a);
}

proc globalDotDomain(a: [A.domain] int, b: [A.domain] int){
  forall i in A.domain {
    a[i] += 
      b[i];
  }
  writeln(a);

  forall i in B.domain {
    a[i] += 
      b[i];
  }
  writeln(a);

  forall i in D {
    a[i] += 
      b[i];
  }
  writeln(a);
}

proc globalDomain(a: [D] int, b: [D] int){
  forall i in A.domain {
    a[i] += 
      b[i];
  }
  writeln(a);

  forall i in B.domain {
    a[i] += 
      b[i];
  }
  writeln(a);

  forall i in D {
    a[i] += 
      b[i];
  }
  writeln(a);
}

proc main() {
  B = 10;

  localQueriedDomain(A, B);

  globalDotDomain(A, B);

  globalDomain(A, B);
}
