module iters {

  // bogus function that the compiler shouldn't eliminate
  proc bogusFunction(arg) { }

  iter myiter(nn: int, nt: int) {
    for i in 0..#nt {
      for j in i*nn..#nn {
        yield j;
      }
    }
  }

  // for loop in standalone without yield should NOT get vector pragma
  // for loop in standalone with yield should get vector pragma
  iter myiter(nn: int, nt: int, param tag: iterKind) where tag == iterKind.standalone {
    coforall i in 0..#nt {
      for j in i*nn..#nn do bogusFunction(j);
      for j in i*nn..#nn {
        yield j;
      }
    }
  }

  // for loop in leader should NOT get vector pragma
  iter myiter(nn: int, nt: int, param tag: iterKind) where tag == iterKind.leader {
    for i in 0..#nt do bogusFunction(i);
    coforall i in 0..#nt {
      yield i*nn..#nn;
    }
  }

  // for loop in follower without yield should NOT get vector pragma
  // for loop in follower with yield should get vector pragma
  iter myiter(nn:int, nt: int, followThis, param tag: iterKind) where tag == iterKind.follower {
    for i in followThis do bogusFunction(i);
    for i in followThis {
      yield i;
    }
  }
}
