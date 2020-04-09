module iters {
  iter myiter(nn: int, nt: int) {
    for i in 0..#nt {
      for j in i*nn..#nn {
        yield j;
      }
    }
  }

  // while do loop in standalone with yield should get vector pragma
  iter myiter(nn: int, nt: int, param tag: iterKind) where tag == iterKind.standalone {
    coforall i in 0..#nt {
      const followThis = i*nn..#nn;
      var  idx = followThis.first;
      const last = followThis.last;
      const str = followThis.stride;
      while (idx != last + str) {
        yield idx;
        idx += str;
      }
    }
  }

  iter myiter(nn: int, nt: int, param tag: iterKind) where tag == iterKind.leader {
    coforall i in 0..#nt {
      yield i*nn..#nn;
    }
  }

  // while do loop in follower with yield should get vector pragma
  // Note that zippered iterator inlining will transform the while into a
  // degenerate CForLoop
  iter myiter(nn:int, nt: int, followThis, param tag: iterKind) where tag == iterKind.follower {
    var  idx = followThis.first;
    const last = followThis.last;
    const str = followThis.stride;
    while (idx != last + str) {
      yield idx;
      idx += str;
    }
  }
}
