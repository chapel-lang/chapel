module iters {

  iter nonInlinableIter(r: range) {
    foreach i in r # (r.size - 1) do 
      yield i;
    yield r.last;
  }

  iter myiter(nn: int, nt: int) {
    foreach i in 0..#nt {
      foreach j in i*nn..#nn {
        yield j;
      }
    }
  }

  // zippered loop in standalone should NOT get vector pragma since one of the
  // iters being zipped can't be inlined
  iter myiter(nn: int, nt: int, param tag: iterKind) where tag == iterKind.standalone {
    coforall i in 0..#nt {
      foreach (j, k) in zip(i*nn..#nn, nonInlinableIter(i*nn..#nn)) {
        yield j;
      }
    }
  }

  iter myiter(nn: int, nt: int, param tag: iterKind) where tag == iterKind.leader {
    coforall i in 0..#nt {
      yield i*nn..#nn;
    }
  }

  // zippered loop in follower should NOT get vector pragma since one of the
  // iters being zipped can't be inlined
  iter myiter(nn:int, nt: int, followThis, param tag: iterKind) where tag == iterKind.follower {
    foreach (i, j) in zip(followThis, nonInlinableIter(followThis)) {
      yield i;
    }
  }
}
