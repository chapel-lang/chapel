module iters {
  // dummy required by fn resolution
  iter myiter(nn: int, nt: int) { yield 0; }

  iter myiter(nn: int, nt: int, param tag: iterKind) where tag == iterKind.leader {
    coforall i in 0..#nt {
      yield i*nn..#nn;
    }
  }

  iter myiter(nn:int, nt: int, followThis, param tag: iterKind) where tag == iterKind.follower {
    for i in followThis {
      yield i;
    }
  }

  // dummy required by fn resolution
  iter nonInlinableIter(nn: int, nt: int) { yield 0; }

  iter nonInlinableIter(nn: int, nt: int, param tag: iterKind) where tag == iterKind.leader {
    coforall i in 0..#nt {
      yield i*nn..#nn;
    }
  }

  iter nonInlinableIter(nn:int, nt: int, followThis, param tag: iterKind) where tag == iterKind.follower {
    for i in followThis # (followThis.size - 1) {
      yield i;
    }
    yield followThis.last;
  }
 
}

module zipIterNoVector {
  proc main() {
    use ItersConfig;
    use iters;
    // The zippered loop should NOT have the vector pragma since one of the
    // zipped iters could not be inlined
    forall (i, j) in zip (myiter(n, ntasks), nonInlinableIter(n, ntasks)) {
      A[i] = i;
      A[j] += j;
    }
    for a in A do writeln(a);
  }
}


