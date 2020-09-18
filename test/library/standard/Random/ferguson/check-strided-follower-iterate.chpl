use Random;

config const debug = false;

iter stridedLeaderIter(n:int, stride:int) {
  for i in 0..#stride {
    for j in i..<n by stride {
      yield j;
    }
  }
}

iter stridedLeaderIter(n:int, stride:int, param tag) where tag == iterKind.leader {
  for i in 0..#stride {
    yield (i..<n by stride,);
  }
}

iter stridedLeaderIter(n:int, stride:int, param tag, followThis) where tag == iterKind.follower {
  for idx in followThis(0) do yield idx;
}


proc test(stride:int) {
  const n = 10*stride;

  var check:[0..<n] int;
  var A:[0..<n] int;
  var B:[0..<n] int;

  for i in stridedLeaderIter(n, stride) {
    check[i] += 1;
    if debug then writeln("for strided leader ", i);
  }

  forall i in stridedLeaderIter(n, stride) {
    check[i] += 1;
    if debug then writeln("forall strided leader ", i);
  }

  for x in check {
    assert(x == 2);
  }

  {
    var rng = new RandomStream(eltType=int, seed=1);
    forall (i, rand) in zip(0..<n, rng.iterate({0..<n})) {
      A[i] = rand; 
    }
  }
   
  {
    var rng = new RandomStream(eltType=int, seed=1);
    forall (i, rand) in zip(stridedLeaderIter(n,stride), rng.iterate({0..<n})) {
      B[i] = rand;
    }
  }

  for (i, a, b) in zip(0..#n, A, B) {
    if debug then writeln("i=", i, " a=", a, " b=", b);
    assert(A[i] == B[i]);
  }
}

for stride in 2..50 {
  test(stride);
}
