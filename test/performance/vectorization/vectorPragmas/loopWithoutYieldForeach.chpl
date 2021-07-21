module iters {

  // bogus function that the compiler shouldn't eliminate
  proc bogusFunction(arg) { }

  iter myiter(nn: int, nt: int) {
    foreach i in 0..#nt {
      foreach j in i*nn..#nn {
        yield j;
      }
    }
  }

  // foreach loop in standalone without yield should get vector pragma
  // foreach loop in standalone with yield should get vector pragma
  iter myiter(nn: int, nt: int, param tag: iterKind) where tag == iterKind.standalone {
    coforall i in 0..#nt {
      foreach j in i*nn..#nn do bogusFunction(j);
      foreach j in i*nn..#nn {
        yield j;
      }
    }
  }

  // foreach loop in leader should get vector pragma
  iter myiter(nn: int, nt: int, param tag: iterKind) where tag == iterKind.leader {
    foreach i in 0..#nt do bogusFunction(i);
    coforall i in 0..#nt {
      yield i*nn..#nn;
    }
  }

  // foreach loop in follower without yield should get vector pragma
  // foreach loop in follower with yield should get vector pragma
  iter myiter(nn:int, nt: int, followThis, param tag: iterKind) where tag == iterKind.follower {
    foreach i in followThis do bogusFunction(i);
    foreach i in followThis {
      yield i;
    }
  }
}
