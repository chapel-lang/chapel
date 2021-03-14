module iters {
  iter myiter(nn: int, nt: int) {
    for i in 0..#nt {
      for j in i*nn..#nn {
        yield j;
      }
    }
  }

  // C for loop in standalone with yield should get vector pragma
  iter myiter(nn: int, nt: int, param tag: iterKind) where tag == iterKind.standalone {
    coforall i in 0..#nt {
      const followThis = i*nn..#nn;
      var idx: followThis.first.type;
      const first = followThis.first;
      const str   = followThis.stride;
      const end   = followThis.last + str;
      while __primitive("C for loop",
                        __primitive( "=", idx, first),
                        __primitive("!=", idx, end),
                        __primitive("+=", idx, str)) {
        yield idx;
      }
    }
  }

  iter myiter(nn: int, nt: int, param tag: iterKind) where tag == iterKind.leader {
    coforall i in 0..#nt {
      yield i*nn..#nn;
    }
  }

  // C for loop in follower with yield should get vector pragma
  iter myiter(nn:int, nt: int, followThis, param tag: iterKind) where tag == iterKind.follower {
    var idx: followThis.first.type;
    const first = followThis.first;
    const str   = followThis.stride;
    const end   = followThis.last + str;
    while __primitive("C for loop",
                      __primitive( "=", idx, first),
                      __primitive("!=", idx, end),
                      __primitive("+=", idx, str)) {
      yield idx;
    }
  }
}
