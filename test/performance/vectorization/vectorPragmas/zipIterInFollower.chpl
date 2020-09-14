module iters {
  iter myiter(nn: int, nt: int) {
    for i in 0..#nt {
      for j in i*nn..#nn {
        yield j;
      }
    }
  }

  // zippered loop in standalone with yield should get vector pragma
  // Note: requires that each iter in the zippered iter gets inlined
  pragma "order independent yielding loops"
  iter myiter(nn: int, nt: int, param tag: iterKind) where tag == iterKind.standalone {
    coforall i in 0..#nt {
      for (j, k) in zip(i*nn..#nn, i*nn..#nn) {
        yield j;
      }
    }
  }

  iter myiter(nn: int, nt: int, param tag: iterKind) where tag == iterKind.leader {
    coforall i in 0..#nt {
      yield i*nn..#nn;
    }
  }

  // zippered loop in standalone with yield should get vector pragma
  // Note: requires that each iter in the zippered iter gets inlined
  pragma "order independent yielding loops"
  iter myiter(nn:int, nt: int, followThis, param tag: iterKind) where tag == iterKind.follower {
    for (i, j) in zip(followThis, followThis) {
      yield i;
    }
  }
}
