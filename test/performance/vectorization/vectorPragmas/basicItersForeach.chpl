module iters {
  // foreach loop in serial should NOT get vector pragma
  iter myiter(nn: int, nt: int) {
    foreach i in 0..#nt {
      foreach j in i*nn..#nn {
        yield j;
      }
    }
  }

  // coforall loop in standalone should NOT get vector pragma
  // foreach loop in standalone with yield should get vector pragma
  iter myiter(nn: int, nt: int, param tag: iterKind) where tag == iterKind.standalone {
    coforall i in 0..#nt {
      foreach j in i*nn..#nn {
        yield j;
      }
    }
  }

  // coforall loop in leader should NOT get vector pragma
  iter myiter(nn: int, nt: int, param tag: iterKind) where tag == iterKind.leader {
    coforall i in 0..#nt {
      yield i*nn..#nn;
    }
  }

  // foreach loop in follower with yield should get vector pragma
  iter myiter(nn:int, nt: int, followThis, param tag: iterKind) where tag == iterKind.follower {
    foreach i in followThis {
      yield i;
    }
  }
}
