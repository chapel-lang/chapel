module iters {
  iter myiter(nn: int, nt: int) {
    foreach i in 0..#nt {
      foreach j in i*nn..#nn {
        yield j;
      }
    }
  }

  // foreach loop in standalone should get vector pragma because standalone iters
  // are force inlined
  iter myiter(nn: int, nt: int, param tag: iterKind) where tag == iterKind.standalone {
    coforall i in 0..#nt {
      foreach j in i*nn..#nn-1 {
        yield j;
      }
      yield (i*nn..#nn).last;
    }
  }

  iter myiter(nn: int, nt: int, param tag: iterKind) where tag == iterKind.leader {
    coforall i in 0..#nt {
      yield i*nn..#nn;
    }
  }

  // foreach loop in follower should NOT get vector pragma because iter can't be
  // inlined (2 yields), and will result in a `advance` function
  iter myiter(nn:int, nt: int, followThis, param tag: iterKind) where tag == iterKind.follower {
      foreach j in followThis # (followThis.size - 1) {
        yield j;
      }
      yield followThis.last;
  }
}
